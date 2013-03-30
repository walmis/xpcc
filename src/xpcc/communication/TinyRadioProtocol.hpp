/*
 * TinyRadioProtocol.hpp
 *
 *      Author: Valmantas Palikša, 2013 <walmis@balticum-tv.lt>
 *
 */

#ifndef RADIO_H_
#define RADIO_H_

#include <memory>
#include <xpcc/workflow.hpp>
#include <xpcc/debug.hpp>
#include <xpcc/container.hpp>
#include <xpcc/architecture.hpp>

#include <xpcc/driver/connectivity/wireless/mac802.15.4/mac.hpp>

#define BEACON_INTERVAL 1000
#define PROTO_HEADER 0x5
#define REQUEST_TIMEOUT 500
#define NUM_RETRIES 12
#define NODE_TIMEOUT 10000 //10s timeout
#define FRAME_HEAP 5

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

struct NodeACL {
	uint16_t address;
	uint32_t session_token;
	uint32_t frame_counter;
	xpcc::Timestamp last_activity;
	bool associated;

	NodeACL() {
		address = 0;
		session_token = 0;
		frame_counter = 0;
		last_activity = xpcc::Clock::now();
		associated = false;
	}
};

//struct Request;
//typedef void (*response_callback)(Request &request, bool result, uint8_t* data,
//		uint8_t len, bool data_pending);

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
	BeaconFrame() {
		hdr.type = BEACON;
		memset(name, 0, sizeof(name));
		memset(data, 0, sizeof(data));
		device_type = DEV_GENERIC;

		hdr.calcChecksum();
	}
} __attribute__((packed));

template <class Driver, class Security>
class TinyRadioProtocol {

public:

	TinyRadioProtocol(Driver& drvr) {
		driver = &drvr;
		self = this;
		last_seq = 0xFF;
		prev_src_addr = 0xFFFE;
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

	void poll() {

		//if(rx_flag) {

		while(!rxFrames.isEmpty()) {
			auto frame = rxFrames.get();
			rxFrames.pop();

			processedFrame = frame;
			processFrame(*frame);
			delete frame;
			processedFrame = 0;

		}

			//rx_flag = false;
		//}

		if(beacon_tm.isExpired()) {
			BeaconFrame bcn;
			prepareBeacon(bcn);

			MacFrame f(tmpFrame);
			prepareFrame(f, 0xFFFF, 0);

			f.addData((uint8_t*)&bcn, sizeof(BeaconFrame));
			RadioStatus res = driver->sendFrame(tmpFrame, true);
			//XPCC_LOG_DEBUG << "\nBeaconTX " << (int)res << xpcc::endl;
			//XPCC_LOG_DEBUG .dump_buffer(frame.data, frame.data_len);
			beacon_tm.restart(BEACON_INTERVAL);
		}

		if(current_request.timer.isActive()) {
			if(current_request.timer.isExpired()) {
				XPCC_LOG_DEBUG << "Request timeout\n";
				current_request.timer.stop();

				if(current_request.request_id == ASSOCIATE_REQ) {
					eventHandler(current_request.address, ASSOCIATION_TIMEOUT);
				}

			}
		}

		//check timeouted nodes
		for(auto node : connectedNodes) {
			if((xpcc::Clock::now() - node->last_activity) > NODE_TIMEOUT) {

				XPCC_LOG_DEBUG .printf("Node timed out\n");

				disassociate(node->address, false);
				break;
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


protected:

	template <typename T>
	ALWAYS_INLINE T* Message(uint8_t* data, uint8_t len) {
		if (XPCC_LOG_LEVEL <= xpcc::log::DEBUG) {
			if(len != sizeof(T)) {
				XPCC_LOG_DEBUG .printf("Type size %d mismatch with provided length %d\n", sizeof(T), len);
			}
		}

		return (T*)data;
	}
	template <typename T>
	ALWAYS_INLINE T* Message(uint8_t* data) {
		return (T*)data;
	}

	virtual void stdResponseHandler(SecureFrame<Security>& frm,
			Request& request, uint8_t* data, uint8_t len);

	virtual void stdRequestHandler(SecureFrame<Security>& frm,
			uint16_t src_address, uint8_t requestId, uint8_t* data, uint8_t len);

	NodeACL* findNode(uint16_t address);
	void removeNode(NodeACL* node);
	NodeACL* createNode();

	virtual void eventHandler(uint16_t address, EventType event) {
		XPCC_LOG_DEBUG .printf("eventHandler(%04x, %d)\n", address, event);
	}

	virtual void requestHandler(SecureFrame<Security> &frm, uint16_t address,
			uint8_t request_type, uint8_t* data, uint8_t len) {

	}

	virtual void responseHandler(SecureFrame<Security> &frm, Request& request,
			uint8_t *data, uint8_t len) {

	}

	virtual void dataHandler(SecureFrame<Security> &frm, FrameHdr& hdr, uint16_t address,
			uint8_t *data, uint8_t len) {

	}

	virtual bool frameHandler(Frame& rxFrame) {
		return true;
	}

	virtual void prepareBeacon(BeaconFrame& frm) {
		frm.device_type = DEV_GENERIC;
		strcpy(frm.name, "WirelessNode");
	}

	virtual void beaconHandler(uint16_t address, const BeaconFrame& bcn) {
		XPCC_LOG_DEBUG .printf("beaconHandler(%04x, %x)\n", address, &bcn);
	}

	static TinyRadioProtocol* self;

	uint16_t address;
	uint16_t panId;

	Driver* driver;

	//xpcc::LinkedList<std::shared_ptr<HeapFrame>> rxFrames;

	xpcc::atomic::Queue<HeapFrame*, FRAME_HEAP> rxFrames;

	Request current_request;
	StaticFrame tmpFrame;
	xpcc::Timeout<> beacon_tm;

	xpcc::LinkedList<NodeACL*> connectedNodes;

	//pointer to currently processed frame
	Frame* processedFrame;

	bool rx_flag;

	uint8_t last_seq;
	uint16_t prev_src_addr;

private:

	void processFrame(Frame& rxFrame);

	static void rxHandler() {

#ifdef FRAME_HEAP
		//XPCC_LOG_DEBUG << "+\n";
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

		//XPCC_LOG_DEBUG .printf("%x %x\n", &f, &self->rxFrames.getBack());
	}

	void prepareFrame(MacFrame& frame, uint16_t dst_addr, uint8_t flags =
			TX_ACKREQ);

	void prepareFrame(SecureFrame<Security>& frame, uint16_t dst_addr,
			uint8_t flags = TX_ACKREQ | TX_ENCRYPT);

};

template<class Driver, class Security>
inline void TinyRadioProtocol<Driver, Security>::processFrame(Frame& rxFrame) {
	//XPCC_LOG_DEBUG .printf("Frame %x\n", &rxFrame);
	SecureFrame<Security> frm(rxFrame);

	if(frm.getSeq() == last_seq && prev_src_addr == frm.getSrcAddress()) {
		XPCC_LOG_DEBUG .printf("Discard duplicate Frame\n");
		return;
	} else {
		last_seq = frm.getSeq();
		prev_src_addr = frm.getSrcAddress();
	}


	uint8_t* payload = frm.getPayload();

	uint8_t size = frm.getPayloadSize();


	NodeACL* node = findNode(frm.getSrcAddress());

	FrameHdr* fr = Message<FrameHdr>(payload);

	if (frm.isSecure()) {
		if (!frm.decrypt()) {

			XPCC_LOG_DEBUG << "Decryption failed >>>" << xpcc::endl;
			XPCC_LOG_DEBUG.dump_buffer(rxFrame.data, rxFrame.data_len);

			//XPCC_LOG_DEBUG.dump_buffer(frm.getPayload(), frm.getPayloadSize());
			XPCC_LOG_DEBUG << "<<<\n";
			return;
		}
		//XPCC_LOG_DEBUG << "Decrypt Frame\n";
		if (node) {
			uint32_t count = frm.getFrameCounter();
			if (count > node->frame_counter) {
				node->frame_counter = count;
			} else {

				XPCC_LOG_DEBUG .printf("Replay attack prevention\n n:%d expected > %d\n", count, node->frame_counter);
				//XPCC_LOG_DEBUG.dump_buffer(rxFrame.data, rxFrame.data_len);

				if (fr->req_id != ASSOCIATE_REQ
						&& fr->type != FrameType::DISSASOC) {

					disassociate(frm.getSrcAddress());
					return;
				}

			}
		}
	}
	//XPCC_LOG_DEBUG << "Frame >>> \n";
	//XPCC_LOG_DEBUG .dump_buffer(rxFrame.data, rxFrame.data_len);
	//XPCC_LOG_DEBUG << "<<< \n";

	//XPCC_LOG_DEBUG.dump_buffer(frm.getPayload(), frm.getPayloadSize());
	//XPCC_LOG_DEBUG << "<<<\n";
	if(node) {
		node->last_activity = xpcc::Clock::now();
	}

	if (!frameHandler(rxFrame)) {
		//drop Frame
		return;
	}
//
//	if (!node && fr->req_id != ASSOCIATE_REQ && fr->req_id != PING_REQ
//			&& fr->type != FrameType::DISSASOC
//			&& fr->type != FrameType::BEACON) {
//
//		XPCC_LOG_DEBUG << "Data from non associated node\n";
//
//	}


	uint8_t *data = payload + sizeof(FrameHdr);
	uint8_t data_len = size - sizeof(FrameHdr);

	if (fr->proto_header != PROTO_HEADER || !fr->check()) {
		XPCC_LOG_DEBUG << "Unknown frm" << xpcc::endl;
		XPCC_LOG_DEBUG.dump_buffer(rxFrame.data, rxFrame.data_len);
	} else {

		if (fr->type == BEACON) {
			BeaconFrame* bcn = Message<BeaconFrame>(payload, size);

			beaconHandler(frm.getSrcAddress(), *bcn);
		}

		//process response
		else if (fr->type == RESPONSE
				&& current_request.address == frm.getSrcAddress()
				&& current_request.timer.isActive()) {

			//XPCC_LOG_DEBUG << "Response " << fr->req_id << xpcc::endl;

			if (fr->req_id == current_request.request_id) {
				if (fr->data_pending) {
					//wait for more data
					current_request.timer.restart(REQUEST_TIMEOUT);
				} else {
					current_request.timer.stop();
				}

				if (fr->req_id < USER_REQ0) {
					stdResponseHandler(frm, current_request, data, data_len);
				} else {
					responseHandler(frm, current_request, data, data_len);
				}

//				if (current_request.cb)
//					current_request.cb(current_request, true,
//							payload + sizeof(FrameHdr), size - sizeof(FrameHdr),
//							fr->data_pending);

			}
		}

		else if(fr->type == DATA) {

			dataHandler(frm, *fr, frm.getSrcAddress(),
						data, data_len);

		}

		else if (fr->type == DISSASOC) {
			uint16_t address = frm.getSrcAddress();

			disassociate(address, false);

		}

		else if (fr->type == REQUEST) {
			//XPCC_LOG_DEBUG << "Request id:" << fr->req_id << xpcc::endl;

			if (fr->req_id < USER_REQ0) {
				stdRequestHandler(frm, frm.getSrcAddress(), fr->req_id, data,
						data_len);
			} else {
				requestHandler(frm, frm.getSrcAddress(), fr->req_id, data,
						data_len);
			}
		}
	}
}



template<class T, class F>
TinyRadioProtocol<T, F>* TinyRadioProtocol<T, F>::self;

template<class Driver, class Security>
inline void TinyRadioProtocol<Driver, Security>::prepareFrame(MacFrame& frame,
		uint16_t dst_addr, uint8_t flags) {
	frame.build();
	frame.setDstAddress(dst_addr, panId);
	frame.setSrcAddress(address);
	frame.ackRequired(flags & TX_ACKREQ);
	frame.setSeq(driver->getStats()->tx_frames);
}

template<class Driver, class Security>
inline void TinyRadioProtocol<Driver, Security>::prepareFrame(SecureFrame<Security>& frame,
		uint16_t dst_addr, uint8_t flags) {
	frame.build();
	frame.setSecure(flags & TX_ENCRYPT);
	frame.setDstAddress(dst_addr, panId);
	frame.setSrcAddress(address);
	frame.ackRequired(flags & TX_ACKREQ);
	frame.setFrameCounter(driver->getStats()->tx_frames);
}

template<class Driver, class Security>
inline NodeACL* TinyRadioProtocol<Driver, Security>::createNode() {
	NodeACL* node = new NodeACL();
	connectedNodes.append(node);
	return node;
}

template<class Driver, class Security>
inline void TinyRadioProtocol<Driver, Security>::removeNode(NodeACL* node) {
	auto it = connectedNodes.begin();
	while (it != connectedNodes.end()) {
		if (*it == node) {
			XPCC_LOG_DEBUG .printf("Removing node %d\n", node->address);
			delete node;
			connectedNodes.remove(it);
			return;
		}
		++it;
	}
}

template<class Driver, class Security>
inline NodeACL* TinyRadioProtocol<Driver, Security>::findNode(uint16_t address) {
	for (auto n : connectedNodes) {
		if (n->address == address) {
			return n;
		}
	}
	return 0;
}

template<class Driver, class Security>
inline void TinyRadioProtocol<Driver, Security>::disassociate(uint16_t address,
		bool send_packet) {
	NodeACL* node = findNode(address);
	if (node) {
		//send disassociation packet
		if (send_packet)
			send(node->address, NULL, 0, DISSASOCIATE_REQ, FrameType::DISSASOC,
					TX_ENCRYPT);

		eventHandler(node->address, DISASSOCIATION_EVENT);
		removeNode(node);
	}
}

template<class Driver, class Security>
inline uint16_t TinyRadioProtocol<Driver, Security>::send(uint16_t address, uint8_t* data,
		uint16_t len, uint8_t req_id, FrameType frm_type, uint8_t flags) {
	uint_fast16_t sz;
	if (!data) {
		//send empty packet
		len = 0;
		goto send;
	}


	while (len > 0) {
send:
		SecureFrame<Security> f(tmpFrame);
		prepareFrame(f, address, flags);
		//XPCC_LOG_DEBUG.dump_buffer(tmpFrame.data, tmpFrame.data_len);

		FrameHdr hdr;

		hdr.req_id = req_id;
		hdr.type = frm_type;

		uint8_t max_size = f.getMaxPayload() - sizeof(FrameHdr);

		if (len > max_size) {
			sz = max_size;

			hdr.data_pending = 1;
		} else {
			sz = len;
			hdr.data_pending = 0;
		}

		hdr.calcChecksum();
		f.addData((uint8_t*) &hdr, sizeof(FrameHdr));
		f.addData(data, sz);

		if (flags & TX_ENCRYPT)
			f.encrypt();
		XPCC_LOG_DEBUG. printf("send() %d %d\n", len, sz);
		//XPCC_LOG_DEBUG << "send() \n";
		//XPCC_LOG_DEBUG.dump_buffer(tmpFrame.data, tmpFrame.data_len);

		RadioStatus res = RadioStatus::TIMED_OUT;
		if ((res = driver->sendFrame(tmpFrame, true)) != RadioStatus::SUCCESS) {
			XPCC_LOG_DEBUG .printf("send failed, res (%d) retrying\n", res);
			for (int i = 0; i < NUM_RETRIES; i++) {
				//resend frame
				res = driver->sendFrame(true);
				if (res == RadioStatus::SUCCESS) {
					XPCC_LOG_DEBUG .printf("Retry successful\n");
					break;
				} else {
					XPCC_LOG_DEBUG .printf("%d .. ", res);
				}

			}
			XPCC_LOG_DEBUG .printf("Fail\n");
		}

		if (res != RadioStatus::SUCCESS) {

			return 0;
		}

		len -= sz;
		data += sz;

	}
	return 1;
}

template<class Driver, class Security>
inline bool TinyRadioProtocol<Driver, Security>::sendResponse(uint8_t* data, uint8_t len,
		uint8_t flags) {
	SecureFrame<Security> f(*processedFrame);
	FrameHdr* hdr = (FrameHdr*) (f.getPayload());
	return send(f.getSrcAddress(), data, len, hdr->req_id, FrameType::RESPONSE,
			flags);
}

template<class Driver, class Security>
inline bool TinyRadioProtocol<Driver, Security>::isAssociated(uint16_t address) {
	auto node = findNode(address);
	if (node && node->associated) {
		return true;
	}
	return false;
}

template<class Driver, class Security>
inline bool TinyRadioProtocol<Driver, Security>::associate(uint16_t address) {
	stdRequests::AssocReq req;
	req.phase = 0;
	req.token = 0;
	return sendRequest<stdRequests::AssocReq>(address, ASSOCIATE_REQ, req, TX_ENCRYPT);
}

template<class Driver, class Security>
inline void TinyRadioProtocol<Driver, Security>::stdRequestHandler(
		SecureFrame<Security>& frm, uint16_t src_address, uint8_t requestId,
		uint8_t* data, uint8_t len) {
	switch (requestId) {
	case PING_REQ: {
		bool result = true;
		sendResponse<bool>(result, TX_ACKREQ | frm.isSecure() ? TX_ENCRYPT : 0);
	}
		break;
	case ASSOCIATE_REQ: {

		//drop insecure requests
		if (!frm.isSecure())
			return;

		auto req = Message<stdRequests::AssocReq>(data);
		XPCC_LOG_DEBUG << "Assoc request phase " << req->phase << xpcc::endl;

		if (req->phase == 0) {
			uint32_t token = xpcc::Random::random();

			NodeACL* n = findNode(src_address);
			if (n) {

				n->session_token = token;
				n->frame_counter = 0;

			} else {
				NodeACL* node = createNode();
				node->address = src_address;
				node->session_token = token;
			}

			stdResponses::AssocResp resp;
			resp.result = 1;
			resp.token = token;

			sendResponse<stdResponses::AssocResp>(resp);

		} else if (req->phase == 1) {

			NodeACL* n = findNode(src_address);
			if (n) {
				stdResponses::AssocResp resp;

				if (n->session_token == req->token) {

					resp.result = 2;
					resp.token = n->session_token;

					n->associated = true;

					eventHandler(n->address, ASSOCIATION_EVENT);

				} else {
					XPCC_LOG_DEBUG << "Association failed, token mismatch\n";
					eventHandler(n->address, DISASSOCIATION_EVENT);

					resp.result = 0;
					removeNode(n);
				}

				sendResponse<stdResponses::AssocResp>(resp);

			}
		}
	}

		break;
	}
}

template<class Driver, class Security>
inline void TinyRadioProtocol<Driver, Security>::stdResponseHandler(
		SecureFrame<Security>& frm, Request& request, uint8_t* data,
		uint8_t len) {
	switch (request.request_id) {
	case ASSOCIATE_REQ: {
		if (!frm.isSecure())
			return;

		//stdResponses::AssocResp *resp = (typeof resp)data;
		auto msg = Message<stdResponses::AssocResp>(data, len);
		if (msg->result == 1) {
			stdRequests::AssocReq req;
			req.phase = 1;
			req.token = msg->token;
			sendRequest<stdRequests::AssocReq>(request.address, ASSOCIATE_REQ,
					req);
		} else if (msg->result == 2) {
			NodeACL* n = findNode(request.address);
			if (!n) {
				n = createNode();
			}
			if (n) {
				n->address = request.address;
				n->session_token = msg->token;
				n->associated = true;
				n->frame_counter = 0;
				eventHandler(n->address, ASSOCIATION_EVENT);
			}
		}
	}
		break;
	}
}

template<class Driver, class Security>
inline bool TinyRadioProtocol<Driver, Security>::sendRequest(uint16_t address,
		uint8_t request_id, uint8_t* data, uint8_t len, uint8_t flags) {
	if (!current_request.timer.isActive()) {

		SecureFrame<Security> f(tmpFrame);
		//requests don't need ack,
		//protocol responses are there anyway
		prepareFrame(f, address, flags);

		FrameHdr hdr;

		hdr.req_id = request_id;
		hdr.type = FrameType::REQUEST;

		hdr.calcChecksum();

		f.addData((uint8_t*) &hdr, sizeof(FrameHdr));

		f.addData(data, len);

		if (flags & TX_ENCRYPT)
			f.encrypt();

		//XPCC_LOG_DEBUG .dump_buffer(frame.data, frame.data_len);

		RadioStatus res = driver->sendFrame(tmpFrame, true);

		if (res == RadioStatus::SUCCESS) {
			current_request.address = address;
			current_request.request_id = request_id;
			current_request.timer.restart(REQUEST_TIMEOUT);
		} else {
			return false;
		}
		return true;

	} else {
		return false;
	}
}

#endif /* RADIO_H_ */
