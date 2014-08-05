/*
 * TinyRadioProtocol_impl.hpp
 *
 *  Created on: Apr 14, 2014
 *      Author: walmis
 */

#include "TinyRadioProtocol.hpp"

namespace xpcc {

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
			RADIO_DEBUG .printf("Removing node %d\n", node->address);
			connectedNodes.remove(it);
			delete node;
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

	} else if(send_packet) {
		send(address, NULL, 0, DISSASOCIATE_REQ, FrameType::DISSASOC,
				TX_ENCRYPT);
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
		StaticFrame frm;
		SecureFrame<Security> f(frm);
		prepareFrame(f, address, flags);
		//RADIO_DEBUG.dump_buffer(tmpFrame.data, tmpFrame.data_len);

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
		//RADIO_DEBUG. printf("send() %d %d\n", len, sz);

		//RADIO_DEBUG << "send() \n";
		//RADIO_DEBUG.dump_buffer(tmpFrame.data, tmpFrame.data_len);

		RadioStatus res = RadioStatus::TIMED_OUT;

		res = transmitFrame(frm);
		if(res == RadioStatus::SUCCESS) {
			while((res = driver->checkTxStatus()) == RadioStatus::TRX_BUSY) {
				TickerTask::yield();
			}
		}

		if (res != RadioStatus::SUCCESS) {
			RADIO_DEBUG .printf("send failed, res (%s) retrying\n", radioStatusStr(res));
			for (int i = 0; i < NUM_RETRIES; i++) {
				//resend frame
				res = transmitFrame();
				while((res = driver->checkTxStatus()) == RadioStatus::TRX_BUSY) {
					TickerTask::yield();
				}
				if (res == RadioStatus::SUCCESS) {
					RADIO_DEBUG .printf("Retry successful\n");
					break;
				} else {
					RADIO_DEBUG .printf("%s .. ", radioStatusStr(res));
				}

			}
			RADIO_DEBUG .printf("\n");
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
	if (node && node->isFlag(ASSOCIATED)) {
		return true;
	}
	return false;
}

template<class Driver, class Security>
inline bool TinyRadioProtocol<Driver, Security>::associate(uint16_t address) {
	stdRequests::AssocReq req;
	req.phase = 0;
	req.token = 0;
	return sendRequest<stdRequests::AssocReq>(address, ASSOCIATE_REQ, req,
			TX_ENCRYPT | TX_ACKREQ);
}

template<class Driver, class Security>
inline void TinyRadioProtocol<Driver, Security>::stdRequestHandler(
		MacFrame& frm, uint16_t src_address, uint8_t requestId,
		uint8_t* data, uint8_t len) {
	switch (requestId) {
	case PING_REQ: {

		sendResponse(data, len, TX_ACKREQ | frm.isSecure() ? TX_ENCRYPT : 0);
	}
		break;
	case ASSOCIATE_REQ: {

		//drop insecure requests
		if (!frm.isSecure())
			return;

		auto req = Message<stdRequests::AssocReq>(data);
		RADIO_DEBUG << "Assoc request phase " << req->phase << xpcc::endl;

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

					n->setFlag(ASSOCIATED, true);

					eventHandler(n->address, ASSOCIATION_EVENT);

				} else {
					RADIO_DEBUG << "Association failed, token mismatch\n";
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
		MacFrame& frm, Request& request, uint8_t* data,
		uint8_t len) {
	switch (request.request_id) {

	case PING_REQ:
	{
		int time = -1;

		if(len == 4) {
			memcpy((uint8_t*)&time, data, 4);
			time = xpcc::Clock::now().getTime() - time;
			if(time < 0) time = -1;
		}

		RADIO_INFO .printf("Ping response from %04x, time=%dms\n", frm.getSrcAddress(), time);
		break;
	}

	case ASSOCIATE_REQ: {
		if (!frm.isSecure()) {
			RADIO_DEBUG .printf("Drop insecure ASSOC frame\n");
			return;
		}

		//stdResponses::AssocResp *resp = (typeof resp)data;
		auto msg = Message<stdResponses::AssocResp>(data, len);
		if (msg->result == 1) {
			RADIO_DEBUG .printf("%04x ASSOC phase 1, token 0x%x\n", frm.getSrcAddress(),
					msg->token);

			stdRequests::AssocReq req;
			req.phase = 1;
			req.token = msg->token;
			sendRequest<stdRequests::AssocReq>(request.address, ASSOCIATE_REQ,
					req, TX_ACKREQ | TX_ENCRYPT);

		} else if (msg->result == 2) {
			NodeACL* n = findNode(request.address);
			if (!n) {
				n = createNode();
			}
			if (n) {
				RADIO_DEBUG .printf("%04x ASSOC success\n", frm.getSrcAddress());

				n->address = request.address;
				n->session_token = msg->token;
				n->setFlag(ASSOCIATED, true);
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

		while(isTxBusy()) {
			TickerTask::yield();
		}

		StaticFrame frm;
		SecureFrame<Security> f(frm);
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

		//RADIO_DEBUG .dump_buffer(frame.data, frame.data_len);
		RadioStatus res;
		for(int i = 0; i < NUM_RETRIES; i++) {
			res = transmitFrame(frm);

			while(isTxBusy()) {
				TickerTask::yield();
			}

			if(res == RadioStatus::SUCCESS) break;
			RADIO_DEBUG .printf("Request (%d) send retry (%s)\n", request_id, radioStatusStr(res));
		}

		if (res == RadioStatus::SUCCESS) {
			current_request.address = address;
			current_request.request_id = request_id;
			current_request.timer.restart(REQUEST_TIMEOUT);
		} else {
			RADIO_DEBUG .printf("Request send failed status:%s\n", radioStatusStr(res));
			return false;
		}
		return true;

	} else {
		return false;
	}
}

template<class Driver, class Security>
inline void TinyRadioProtocol<Driver, Security>::processFrame(Frame& rxFrame) {
	//RADIO_DEBUG .printf("Frame %x\n", &rxFrame);
	SecureFrame<Security> frm(rxFrame);

	bool found = false;
	for (auto &f : frameSeqCounters) {

		if(f.address == frm.getSrcAddress()) {
			if(frm.getSeq() == f.last_seq) {
				RADIO_WARNING .printf("Discard duplicate Frame (%04x, %d)\n",
						f.address, frm.getSeq());

				return;
			}

			f.last_seq = frm.getSeq();

			found = true;
		}
	}
	if(!found) {
		FrameCounter c;
		c.address = frm.getSrcAddress();
		c.last_seq = frm.getSeq();

		frameSeqCounters.append(c);
	}

	uint8_t* payload = frm.getPayload();
	uint8_t size = frm.getPayloadSize();

	NodeACL* node = findNode(frm.getSrcAddress());

	if(node) {
		node->last_activity = xpcc::Clock::now();
	}

	FrameHdr* fr = Message<FrameHdr>(payload);

	//RADIO_DEBUG .printf("Frame (len:%d)>>> \n", rxFrame.data);
	//RADIO_DEBUG .dump_buffer(rxFrame.data, rxFrame.data_len);
	//RADIO_DEBUG << "<<< \n";

	if (frm.isSecure()) {
		if (!frm.decrypt()) {

			RADIO_ERROR << "Decryption failed >>>" << xpcc::endl;
			RADIO_DEBUG.dump_buffer(rxFrame.data, rxFrame.data_len);

			//RADIO_DEBUG.dump_buffer(frm.getPayload(), frm.getPayloadSize());
			RADIO_DEBUG << "<<<\n";
			return;
		}
		//RADIO_DEBUG << "Decrypt Frame\n";
		if (node) {
			uint32_t count = frm.getFrameCounter();
			if (count > node->frame_counter) {
				node->frame_counter = count;
			} else {

				RADIO_WARNING .printf("Replay attack prevention\n n:%d expected > %d\n", count, node->frame_counter);
				//RADIO_DEBUG.dump_buffer(rxFrame.data, rxFrame.data_len);

				if (fr->req_id != ASSOCIATE_REQ
						&& fr->type != FrameType::DISSASOC) {

					disassociate(frm.getSrcAddress());
					return;
				}

			}
		}
	}

	//RADIO_DEBUG.dump_buffer(frm.getPayload(), frm.getPayloadSize());
	//RADIO_DEBUG << "<<<\n";

	if (!frameHandler(rxFrame)) {
		//drop Frame
		return;
	}

	uint8_t *data = payload + sizeof(FrameHdr);
	uint8_t data_len = size - sizeof(FrameHdr);

	if (fr->proto_header != PROTO_HEADER || !fr->check()) {
		RADIO_DEBUG << "Unknown frm" << xpcc::endl;
		RADIO_DEBUG.dump_buffer(rxFrame.data, rxFrame.data_len);
	} else {

		if (fr->type == BEACON) {
			BeaconFrame* bcn = Message<BeaconFrame>(payload, size);

			beaconHandler(frm.getSrcAddress(), *bcn);
		}

		//process response
		else if (fr->type == RESPONSE
				&& current_request.address == frm.getSrcAddress()
				&& current_request.timer.isActive()) {

			//RADIO_DEBUG << "Response " << fr->req_id << xpcc::endl;

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

			} else {
				RADIO_DEBUG .printf("Got response for unknown request\n");
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
			//RADIO_DEBUG << "Request id:" << fr->req_id << xpcc::endl;

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

} //namespace xpcc
