/* Copyright (c) 2013, Valmantas Palikša <walmis@balticum-tv.lt>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MACFRAME_H_
#define MACFRAME_H_

#include "Frame.h"
#include <xpcc/debug.hpp>

class MacFrame {
public :

	struct FcfHeader{
		uint16_t frame_type : 3;
		uint16_t securityEnabled : 1;
		uint16_t framePending : 1;
		uint16_t ackRequred : 1;
		uint16_t panCompression : 1;
		uint16_t : 3;
		uint16_t dstAddressingMode : 2;
		uint16_t : 2;
		uint16_t srcAddressingMode : 2;
	} __attribute__((packed));

	enum FcfFrameType {
		BEACON = 0,
		DATA = 1,
		ACK = 2,
		MAC_COMMAND = 3
	};


	enum AddrMode {
		ADDRESS_NOT_PRESENT = 0,
		RESERVED0,
		ADDRESS_16BIT,
		ADDRESS_64BIT_EXTENDED
	};

	MacFrame(Frame& f) {
		assign(f);
	}

	const uint8_t header_sz = 9;
	const uint8_t frame_sz = 127;
	const uint8_t fcs_sz = 2;

	void assign(Frame& frm) {
		frame = &frm;
		//payload_len = 0;
		if(frame->rx_flag) {
			//payload_len = frame->data_len - (getPayload() - frame->data);
		} else {
			frame->data_len = header_sz;
			frame->data[0] = 0; //clear FCF header
			frame->data[1] = 0;
			setType();
		}

	}

	void setType(FcfFrameType type = FcfFrameType::DATA) {
		FcfHeader* hdr = (FcfHeader*)frame->data;
		hdr->frame_type = type;
	}

	void ackRequired(bool value) {
		getFCF()->ackRequred = (int)value;
	}

	Frame* getFrame() {
		return frame;
	}

	uint16_t getSrcAddress();

	uint16_t getSrcPan();

	uint16_t getDstAddress() {
		return getWord(frame->data + 5);
	}

	uint16_t getDstPan() {
		return getWord(frame->data + 3);
	}

	void build() {
		clear();

		setType(MacFrame::FcfFrameType::DATA);
		ackRequired(false);
	}


	void setSrcAddress(uint16_t address, uint16_t panId = 0xFFFF);

	void setDstAddress(uint16_t address, uint16_t panId = 0xFFFF);

	void setSeq(uint8_t seq) {
		frame->data[2] =  seq;
	}

	uint8_t getSeq() {
		return frame->data[2];
	}

	virtual uint8_t getMaxPayload() {
		return frame->buffer_size - header_sz - fcs_sz;
	}

	virtual uint8_t* getPayload() {
		return frame->data + header_sz;
	}

	virtual uint8_t getPayloadSize() {
		int16_t s = frame->data_len - (getPayload() - frame->data);
		if(s < 0) return 0;
		return s;
	}

	void addData(uint8_t* data, uint8_t len);

	void clear();

protected:
	Frame* frame;
	//uint8_t payload_len;

	inline FcfHeader* getFCF() {
		FcfHeader* hdr = (FcfHeader*)frame->data;
		return hdr;
	}

};



#endif /* MACFRAME_H_ */
