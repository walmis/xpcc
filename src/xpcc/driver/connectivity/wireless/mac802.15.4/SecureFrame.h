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

#ifndef SECUREFRAME_H_
#define SECUREFRAME_H_

#include "MacFrame.h"

template <class Codec>
class SecureFrame : public MacFrame {
public:

	enum class SecurityMode : uint8_t {
		NO_SECURITY = 0x00,
		AES_CBC_MAC_32 = 0x01,
		AES_CBC_MAC_64 = 0x02,
		AES_CBC_MAC_128 = 0x03,
		AES_CTR = 0x04,
		AES_CCM_32 = 0x05,
		AES_CCM_64 = 0x06,
		AES_CCM_128 = 0x07
	};


	struct SecHeader {
		uint8_t security_level : 3;
		uint8_t key_id : 2;
		uint8_t : 3;

		uint32_t frame_counter;

	} __attribute__((packed));

	SecureFrame(Frame& f) : MacFrame(f) {

	}
	uint8_t getKeyCounter() {
		return 0;
	}

	void build() {
		MacFrame::build();

		uint8_t* payload = MacFrame::getPayload();

		SecHeader* hdr = (SecHeader*)payload;

		hdr->security_level = codec.securityMode;
		hdr->key_id = 0;
		hdr->frame_counter = 0;

		setType();
	}

	void clear() {
		getFCF()->securityEnabled = false;

		MacFrame::clear();
	}

	void setSecure(bool secure) {
		if(secure) {
			if(!isSecure()) {
				frame->data_len += (5 + codec.getHeaderSize() + codec.getFooterSize());
				getFCF()->securityEnabled = true;
			}
		} else {
			if(isSecure()) {
				frame->data_len -= (5 + codec.getHeaderSize() + codec.getFooterSize());
				getFCF()->securityEnabled = false;
			}
		}
	}

//	void assign(Frame& frm) {
//		MacFrame::assign(frm);
//		printf("issecure %d %d\n", isSecure(), payload_len);
//		if(frame->rx_flag) {
//
//			if(isSecure()) {
//				if(getHdr()->security_level == codec.securityMode) {
//					payload_len -= codec.getFooterSize();
//				}
//			}
//		}
//	}

	void encrypt() {
		if(isSecure())
			codec.encrypt(*this);
	}

	bool decrypt() {
		if(isSecure())
			return codec.decrypt(*this);
		else
			return false;
	}

	uint32_t getFrameCounter() {
		return getHdr()->frame_counter;
	}

	void setFrameCounter(uint32_t count) {
		setSeq(count);
		getHdr()->frame_counter = count;
	}

	uint8_t getMaxPayload() {
		if(isSecure()) {
			if(getHdr()->security_level == codec.securityMode) {
				return MacFrame::getMaxPayload() - 5 - (codec.getHeaderSize() + codec.getFooterSize());
			} else {
				return MacFrame::getMaxPayload() - 5;
			}
		}
		return MacFrame::getMaxPayload();
	}

	uint8_t* getPayload() {
		if(isSecure()) {
			if(getHdr()->security_level == codec.securityMode) {
				return MacFrame::getPayload() + 5 + codec.getHeaderSize();
			} else {
				return MacFrame::getPayload() + 5;
			}
		}
		return MacFrame::getPayload();
	}

	uint8_t getPayloadSize() {
		if(isSecure()) {
			return MacFrame::getPayloadSize() - codec.getFooterSize();
		}
		return MacFrame::getPayloadSize();
	}

private:
	Codec codec;

	inline SecHeader* getHdr() {
		uint8_t* payload = MacFrame::getPayload();
		return (SecHeader*)payload;
	}

};

#endif /* SECUREFRAME_H_ */
