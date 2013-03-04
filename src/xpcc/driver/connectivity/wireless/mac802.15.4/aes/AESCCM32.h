/*
 * AESCCM32.h
 *
 *  Created on: Feb 19, 2013
 *      Author: walmis
 */

#ifndef AESCCM32_H_
#define AESCCM32_H_

#include "aes.h"
#include "../SecureFrame.h"

struct Nonce {
	uint32_t src_address;
	uint32_t rand;
	uint32_t frame_counter;
	uint8_t key_counter;
} __attribute__((packed));

class AES_CCM_32 {
public:
	const uint8_t securityMode = (uint8_t)SecureFrame<AES_CCM_32>::SecurityMode::AES_CCM_32;

private:
	static aes128_ctx_t ctx;

public:
	static const int blocksize = 16; //128bit
	static const int M = 4; //mac size
	static const int nonce_size = 13;
	static const int L = blocksize - nonce_size - 1; //2

	constexpr uint8_t getHeaderSize() {
		return 5;
	}
	constexpr uint8_t getFooterSize() {
		return 4;
	}


	static void setKey(const uint8_t* key) {
		aes128_init(key, &ctx);
	}

	void encrypt(SecureFrame<AES_CCM_32>& frame);

	bool decrypt(SecureFrame<AES_CCM_32>& frame);

	void encrypt(uint8_t* buf, uint16_t len, const Nonce& nonce);

	bool decrypt(uint8_t* buf, uint16_t len, const Nonce& nonce);

private:
	static void _ctr_block(uint8_t* ptr, const Nonce& nonce, uint16_t counter,
			uint8_t flags);

	void calc_mic(const uint8_t* buf, uint16_t len, uint8_t* mic,
			const Nonce& nonce);

	static void block_xor(const uint8_t* a, uint8_t* bo,
			uint_fast16_t len = 16);


};



#endif /* AESCCM32_H_ */
