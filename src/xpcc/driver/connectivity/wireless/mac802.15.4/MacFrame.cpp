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


#include "MacFrame.h"

#include <string.h>

void MacFrame::addData(uint8_t* data, uint8_t len) {
	if (!frame->rx_flag) {
		int l = getMaxPayload() - getPayloadSize();
		if (len > l)
			len = l;

		memcpy(getPayload() + getPayloadSize(), data, len);
		//payload_len += len;
		frame->data_len += len;
	}
}

void MacFrame::clear() {
	memset(MacFrame::getPayload(), 0, MacFrame::getMaxPayload());
	frame->rx_flag = 0;

	assign(*frame);
}

uint16_t MacFrame::getSrcAddress() {
	FcfHeader* hdr = (FcfHeader*) (frame->data);
	if (hdr->panCompression) {
		return getWord(frame->data + 7);
	} else {
		return getWord(frame->data + 9);
	}
}

uint16_t MacFrame::getSrcPan() {
	FcfHeader* hdr = (FcfHeader*) (frame->data);
	if (hdr->panCompression) {
		return 0xFFFF;
	} else {
		return getWord(frame->data + 6);
	}
}

void MacFrame::setSrcAddress(uint16_t address, uint16_t panId) {
	FcfHeader* hdr = (FcfHeader*) (frame->data);
	hdr->srcAddressingMode = AddrMode::ADDRESS_16BIT;
	int offset = 7;

	if (panId == 0xFFFF)
		hdr->panCompression = 1;
	else {
		putWord(frame->data+offset, panId);

		offset += 2;
	}
	putWord(frame->data+offset, address);

}

void MacFrame::setDstAddress(uint16_t address, uint16_t panId) {
	FcfHeader* hdr = (FcfHeader*) (frame->data);
	hdr->dstAddressingMode = AddrMode::ADDRESS_16BIT;

	putWord(frame->data+3, panId);
	putWord(frame->data+5, address);

}
