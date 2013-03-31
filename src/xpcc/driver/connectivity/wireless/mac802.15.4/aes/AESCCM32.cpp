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

#include "AESCCM32.h"

#include <stdlib.h>
#include <string.h>
#include <xpcc/architecture.hpp>


void AES_CCM_32::encrypt(SecureFrame<AES_CCM_32>& frame) {
	uint8_t* buf = frame.getPayload() - getHeaderSize();
	uint8_t len = frame.getPayloadSize();

	Nonce nonce;
	memset(&nonce, 0, sizeof(Nonce));

	nonce.src_address = frame.getSrcAddress();
	nonce.rand = rand();
	nonce.frame_counter = frame.getFrameCounter();
	nonce.key_counter = frame.getKeyCounter();

	buf[4] = nonce.key_counter;

	memcpy(buf, &nonce.rand, sizeof(uint32_t));

	buf = frame.getPayload();
	Frame *fr = frame.getFrame();
	calc_mic(fr->data, (buf + len) - fr->data, buf + len, nonce);


	encrypt(buf, len, nonce);
}

void AES_CCM_32::encrypt(uint8_t* buf, uint16_t len,
		const Nonce& nonce) {
	int tmp, offset, counter;
	uint8_t block[16];

	tmp = len;
	offset = 0;
	counter = 0;
	while (tmp > 0) {
		counter++;
		_ctr_block(block, nonce, counter, L - 1);

		aes128_enc(block, &ctx);
		block_xor(block, buf + offset, (tmp < 16) ? tmp : 16);
		offset += 16;
		tmp -= 16;
	}
}

bool AES_CCM_32::decrypt(SecureFrame<AES_CCM_32>& frame) {
	uint8_t* buf = frame.getPayload() - getHeaderSize();
	uint8_t len = frame.getPayloadSize();
	Nonce nonce;
	memset(&nonce, 0, sizeof(Nonce));

	nonce.src_address = frame.getSrcAddress();
	memcpy(&nonce.rand, buf, sizeof(uint32_t));

	//get frame counter
	nonce.frame_counter = frame.getFrameCounter();

	nonce.key_counter = buf[4];

	buf = frame.getPayload();
	decrypt(buf, len, nonce);

	uint8_t mic[4];

	Frame *fr = frame.getFrame();
	calc_mic(fr->data, (buf + len) - fr->data, mic, nonce);
	//XPCC_LOG_DEBUG .printf("len %d\n", frame.getPayloadSize());
	//XPCC_LOG_DEBUG .dump_buffer(mic, 4);
	//XPCC_LOG_DEBUG .dump_buffer(buf + len, 4);
	//XPCC_LOG_DEBUG .dump_buffer(buf, len);

	if (memcmp(mic, buf + len, M) == 0) {
		return true;
	}
	return false;
}

bool AES_CCM_32::decrypt(uint8_t* buf, uint16_t len, const Nonce& nonce) {
	int tmp;
	uint8_t block[16];
	tmp = len;
	int counter = 1;
	uint8_t* ptr = buf;
	while (tmp > 0) {
		_ctr_block(block, nonce, counter, L - 1);
		aes128_enc(block, &ctx);
		block_xor(block, ptr, (tmp < 16) ? tmp : 16);
		tmp -= 16;
		ptr += 16;
		counter++;
	}

	return true;
}

void AES_CCM_32::_ctr_block(uint8_t* block, const Nonce& nonce,
		uint16_t counter, uint8_t flags) {
	*block++ = flags;
	memcpy(block, &nonce, sizeof(Nonce));
	block += sizeof(Nonce);
	*block++ = counter >> 8;
	*block = counter;
}

void AES_CCM_32::calc_mic(const uint8_t* buf, uint16_t len, uint8_t* mic,
		const Nonce& nonce) {
	int tmp;
	uint8_t* ptr;
	uint8_t block[blocksize];
	uint8_t mic_value[blocksize];
	const uint8_t Adata = 0;
	const uint8_t Mfield = ((M - 2) / 2) << 3; // bits 5,4,3 encode macSize
	const uint8_t Lfield = L - 1; //bits 2,1,0 encode L size = blockSize-nonceSize-1
	const uint8_t flagsByte = Adata ^ Mfield ^ Lfield;
	_ctr_block(mic_value, nonce, len, flagsByte);
	aes128_enc(mic_value, &ctx);
	//TODO: addAuthMessage
	aes128_enc(mic_value, &ctx);
	tmp = len;
	ptr = (uint8_t*) (buf);
	while (tmp > 0) {
		block_xor(ptr, mic_value, (tmp < 16) ? tmp : 16);
		aes128_enc(mic_value, &ctx);
		ptr += 16;
		tmp -= 16;
	}
	int counter = 0;
	_ctr_block(block, nonce, counter, L - 1);
	//print(block);
	aes128_enc(block, &ctx);
	block_xor(mic_value, block);
	memcpy(mic, block, 4);
}

void AES_CCM_32::block_xor(const uint8_t* a, uint8_t* bo,
		uint_fast16_t len) {
	//modulus operation
	//PROFILE();

	//check alignment and length is divisible by 4
	if ((len & (4 - 1)) == 0 && (((long)a & 3) | ((long)bo & 3)) == 0) {
		//use 32bit words for speed
		for (int i = 0; i < (len >> 2); i++) {
			((uint32_t*) (bo))[i] = ((uint32_t*) (a))[i]
					^ ((uint32_t*) (bo))[i];
		}
		return;
	}
	for (int i = 0; i < len; i++) {
		bo[i] = a[i] ^ bo[i];
	}
}

aes128_ctx_t AES_CCM_32::ctx;
