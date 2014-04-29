/*
 * TinyRadioProtocol.hpp
 *
 *      Author: Valmantas Palikša, 2013 <walmis@balticum-tv.lt>
 *
 */

#ifndef RADIO_H_
#define RADIO_H_

#include <memory>
#include <xpcc/processing.hpp>
#include <xpcc/debug.hpp>
#include <xpcc/container.hpp>
#include <xpcc/architecture.hpp>

#include <xpcc/driver/connectivity/wireless/mac802.15.4/mac.hpp>

#ifndef TRP_DEBUG
#define RADIO_DEBUG XPCC_LOG_OFF
#define RADIO_INFO XPCC_LOG_OFF
#define RADIO_WARNING XPCC_LOG_OFF
#define RADIO_ERROR XPCC_LOG_OFF

#else
#ifndef RADIO_DEBUG
#define RADIO_DEBUG XPCC_LOG_DEBUG
#endif

#define RADIO_INFO XPCC_LOG_INFO
#define RADIO_WARNING XPCC_LOG_WARNING
#define RADIO_ERROR XPCC_LOG_ERROR

#endif

#define BEACON_INTERVAL 1000
#define PROTO_HEADER 0x5
#define REQUEST_TIMEOUT 150
#define NUM_RETRIES 12

#ifndef NODE_TIMEOUT
#define NODE_TIMEOUT 5000 //2.5s timeout
#endif

#define NODE_TIMEOUT_PING 2500

#define FRAME_HEAP 5

namespace xpcc {
namespace stdResponses {

struct AssocResp {
	uint8_t result;
	uint32_t token;
} __attribute__((packed));

}
namespace stdRequests {

struct AssocReq {
	uint8_t phase;
	uint32_t token;
} __attribute__((packed));

}

enum DeviceFlags : uint8_t {
	ASSOCIATED = 1<<0, // flag is set when device is associated
	INACTIVE = 1<<1, //device is flagged inactive, when there
					 //are no beacons in NODE_TIMEOUT interval
	PINGED = 1<<2    //Device is flagged PINGED when it is
					//inactive and a ping is sent to determine if it is still alive
};

struct NodeACL {
	uint16_t address;
	uint32_t session_token;
	uint32_t frame_counter;
	xpcc::Timestamp last_activity;
	uint8_t flags; //device flags

	NodeACL() {
		address = 0;
		session_token = 0;
		frame_counter = 0;
		last_activity = xpcc::Clock::now();
		flags = (DeviceFlags)0;
	}

	bool isAssociated() {
		return isFlag(ASSOCIATED);
	}

	inline bool isFlag(DeviceFlags flag) {
		return flags & (uint8_t)flag;
	}

	inline void setFlag(DeviceFlags flag, bool value) {
		flags &= ~(uint8_t)flag;
		if(value)
			flags |= (uint8_t)flag;
	}

};

struct FrameCounter {
	uint16_t address;
	uint8_t last_seq;
};

struct Request {
	uint16_t address;
	uint8_t request_id;
	xpcc::Timeout<> timer;

	Request() {
		address = 0;
		request_id = 0;
		timer.stop();
		//cb = nullptr;
	}
};

enum StdRequest {
	PING_REQ = 1,
	ASSOCIATE_REQ,
	DISSASOCIATE_REQ,


	USER_REQ0 = 16,
};

enum EventType {
	ASSOCIATION_EVENT,
	DISASSOCIATION_EVENT,
	ASSOCIATION_TIMEOUT
};

enum TxFlags : uint8_t {
	TX_ENCRYPT = 1<<0,
	TX_ACKREQ = 1<<1,
};


enum FrameType {
	BEACON = 0x00,
	DATA = 0x01,
	REQUEST = 0x02,
	RESPONSE = 0x03,
	DISSASOC = 0x04
};

enum DeviceType {
	DEV_GENERIC = 0
};

struct FrameHdr {
	uint8_t proto_header : 4;
	uint8_t type : 3;
	uint8_t data_pending : 1;

	uint8_t req_id; //request ID
	uint8_t len;
	uint8_t checksum;

	FrameHdr() {
		proto_header = PROTO_HEADER;
		type = DATA;
		data_pending = 0;
		checksum = 0;

		len = 0;
		req_id = 0;
	}

	void calcChecksum() {
		checksum = 0x55 ^ proto_header ^ type ^ data_pending ^ req_id ^ len;
	}

	bool check() {
		return (0x55 ^ proto_header ^ type ^ data_pending ^ req_id ^ len) == checksum;
	}

} __attribute__((packed));

struct BeaconFrame {
	FrameHdr hdr;
	uint8_t device_type;
	char name[16];
	char data[8];

	char* getName() {
		return name;
	}

	char* getData() {
		return data;
	}

	BeaconFrame() {
		hdr.type = BEACON;
		memset(name, 0, sizeof(name));
		memset(data, 0, sizeof(data));
		device_type = DEV_GENERIC;

		hdr.calcChecksum();
	}
} __attribute__((packed));


/**
* \brief		TinyRadioProtocol

* \ingroup		communication
*/

/**
 * A small radio protocol implementation based on IEEE 802.15.4
 *
 * */

template <class Driver, class Security>
class TinyRadioProtocol : xpcc::TickerTask {

public:

	TinyRadioProtocol(Driver& drvr) {
		driver = &drvr;
		self = this;
		last_seq = 0xFF;
		rx_flag = false;
		address = 0;
		panId = 0;
	}
	virtual ~TinyRadioProtocol() {

	}


	void init() {
		rx_flag = false;
		driver->init();
		driver->setRxFrameHandler(rxHandler);

		beacon_tm.restart(BEACON_INTERVAL);

	}

	void setAddress(uint16_t src_address) {
		driver->setShortAddress(src_address);
		address = src_address;
	}
	void setPanId(uint16_t panid) {
		driver->setPanId(panid);
		panId = panid;
	}

	uint16_t getAddress() {
		return address;
	}

	inline RadioStatus transmitFrame(Frame& frm, bool blocking = false) {
		Timeout<> t(10);
		while(isTxBusy()) {
			if(t.isExpired()) {
				return RadioStatus::TIMED_OUT;
			}
			TickerTask::yield();
		}

		return driver->sendFrame(frm, blocking);

	}

	//send a frame that is already uploaded to the frame buffer
	inline RadioStatus transmitFrame(bool blocking = false) {
		Timeout<> t(10);
		while(isTxBusy()) {
			if(t.isExpired()) {
				return RadioStatus::TIMED_OUT;
			}
			TickerTask::yield();
		}
		return driver->sendFrame(blocking);
	}

	inline bool isTxBusy() {
		return driver->checkTxStatus() == RadioStatus::TRX_BUSY;
	}

	void handleTick() override {

		while(!rxFrames.isEmpty()) {
			auto frame = rxFrames.get();
			rxFrames.pop();

			processedFrame = frame;
			processFrame(*frame);
			delete frame;
			processedFrame = 0;

		}

		if(beacon_tm.isExpired() && !isTxBusy()) {
			BeaconFrame bcn;
			prepareBeacon(bcn);

			StaticFrame tmp;
			MacFrame f(tmp);
			prepareFrame(f, 0xFFFF, 0);

			f.addData((uint8_t*)&bcn, sizeof(BeaconFrame));
			RadioStatus res = transmitFrame(tmp);

			if(res != RadioStatus::SUCCESS) {
				RADIO_ERROR << "BeaconTX failure " << res << xpcc::endl;
			} else {
				beacon_tm.restart(BEACON_INTERVAL);
			}
		}

		if(current_request.timer.isActive()) {
			if(current_request.timer.isExpired()) {
				RADIO_DEBUG .printf("Request %d -> %04x timeout\n",
						current_request.request_id, current_request.address);

				if(current_request.request_id == PING_REQ) {

					//clear pinged flag if ping times out
					//this will cause another ping request to be sent
					auto *n = findNode(current_request.address);
					if(n) {
						if(n->isFlag(PINGED)) {
							n->setFlag(PINGED, false);
						}
					}
				}

				current_request.timer.stop();

				if(current_request.request_id == ASSOCIATE_REQ) {
					eventHandler(current_request.address, ASSOCIATION_TIMEOUT);
				}

			}
		}

		//check timeouted nodes
		for(auto node : connectedNodes) {

			xpcc::Timestamp delta = xpcc::Clock::now() - node->last_activity;

			if(delta > NODE_TIMEOUT_PING) {

				if(!node->isFlag(PINGED)) {
					RADIO_INFO .printf("Pinging inactive node %04x\n", node->address);

					ping(node->address);

					node->setFlag(PINGED, true);
					node->setFlag(INACTIVE, true);
				}
			} else {
				if(node->isFlag(INACTIVE)) {
					node->setFlag(INACTIVE, false);
					node->setFlag(PINGED, false);
				}
			}

			if(delta > NODE_TIMEOUT && node->isFlag(PINGED)) {

				RADIO_WARNING .printf("Node %04x timed out\n", node->address);

				disassociate(node->address, false);
			}

		}

	}

	bool sendRequest(uint16_t address, uint8_t request_id, uint8_t* data,
			uint8_t len, uint8_t flags = TX_ENCRYPT);

	template<typename T>
	ALWAYS_INLINE bool sendRequest(uint16_t address, uint8_t request_id,
			T& request, uint8_t flags = TX_ENCRYPT) {

		return sendRequest(address, request_id, (uint8_t*)&request, sizeof(T), flags);
	}

	bool ping(uint16_t address) {
		uint32_t time = (int)xpcc::Clock::now().getTime();
		return sendRequest(address, PING_REQ, time, TX_ACKREQ | TX_ENCRYPT);
	}

	uint16_t send(uint16_t address, uint8_t* data, uint16_t len,
			uint8_t req_id = 0, FrameType frm_type = FrameType::DATA,
			uint8_t flags = TX_ENCRYPT | TX_ACKREQ);


	bool sendResponse(uint8_t* data, uint8_t len,
			uint8_t flags = TX_ENCRYPT | TX_ACKREQ);


	template <typename T>
	ALWAYS_INLINE bool sendResponse(T& data, uint8_t flags = TX_ENCRYPT | TX_ACKREQ) {
		return sendResponse((uint8_t*)&data, sizeof(T), flags);
	}


	bool associate(uint16_t address);
	void disassociate(uint16_t address, bool send_packet = true);
	bool isAssociated(uint16_t address);

	NodeACL* findNode(uint16_t address);

	xpcc::LinkedList<NodeACL*> connectedNodes;

protected:

	template <typename T>
	ALWAYS_INLINE T* Message(uint8_t* data, uint8_t len) {
		if (XPCC_LOG_LEVEL <= xpcc::log::DEBUG) {
			if(len != sizeof(T)) {
				RADIO_DEBUG .printf("Type size %d mismatch with provided length %d\n", sizeof(T), len);
			}
		}

		return (T*)data;
	}
	template <typename T>
	ALWAYS_INLINE T* Message(uint8_t* data) {
		return (T*)data;
	}

	virtual void stdResponseHandler(MacFrame& frm,
			Request& request, uint8_t* data, uint8_t len);

	virtual void stdRequestHandler(MacFrame& frm,
			uint16_t src_address, uint8_t requestId, uint8_t* data, uint8_t len);


	void removeNode(NodeACL* node);
	NodeACL* createNode();

	virtual void eventHandler(uint16_t address, EventType event) {
		RADIO_DEBUG .printf("eventHandler(%04x, %d)\n", address, event);
	}

	virtual void requestHandler(MacFrame &frm, uint16_t address,
			uint8_t request_type, uint8_t* data, uint8_t len) {}

	virtual void responseHandler(MacFrame &frm, Request& request,
			uint8_t *data, uint8_t len) {}

	virtual void dataHandler(MacFrame &frm, FrameHdr& hdr, uint16_t address,
			uint8_t *data, uint8_t len) {}

	virtual bool frameHandler(Frame& rxFrame) {
		return true;
	}

	virtual void prepareBeacon(BeaconFrame& frm) {
		frm.device_type = DEV_GENERIC;
		strcpy(frm.name, "WirelessNode");
	}

	virtual void beaconHandler(uint16_t address, const BeaconFrame& bcn) {
		RADIO_DEBUG .printf("beaconHandler(%04x, %x)\n", address, &bcn);
	}

	static TinyRadioProtocol* self;

	uint16_t address;
	uint16_t panId;

	Driver* driver;

	xpcc::LinkedList<FrameCounter> frameSeqCounters;

	xpcc::atomic::Queue<HeapFrame*, FRAME_HEAP> rxFrames;

	Request current_request;

	xpcc::Timeout<> beacon_tm;

	//pointer to currently processed frame
	Frame* processedFrame;

	bool rx_flag;

	uint8_t last_seq;

	void processFrame(Frame& rxFrame);

	static void rxHandler() {

#ifdef FRAME_HEAP
		//RADIO_DEBUG << "+\n";
		if(!self->rxFrames.isFull()) {
			HeapFrame* frm = new HeapFrame;
			if(frm && frm->allocate(self->driver->getFrameLength())) {
				self->driver->readFrame(*frm);
				self->rxFrames.push(frm);
				//self->rx_flag = true;
			}
		}

#else
		if(!self->rx_flag) {
			self->driver->readFrame(self->frame);
			self->rx_flag = true;
		}
#endif

		//RADIO_DEBUG .printf("%x %x\n", &f, &self->rxFrames.getBack());
	}

	void prepareFrame(MacFrame& frame, uint16_t dst_addr, uint8_t flags =
			TX_ACKREQ);

	void prepareFrame(SecureFrame<Security>& frame, uint16_t dst_addr,
			uint8_t flags = TX_ACKREQ | TX_ENCRYPT);

};

}

#include "TinyRadioProtocol_impl.hpp"

#endif /* RADIO_H_ */
