/*
 * aes.h
 *
 *  Created on: Mar 31, 2012
 *      Author: walmis
 */

#ifndef AES_H_
#define AES_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// AES only supports Nb=4
#define Nb 4			// number of columns in the state & expanded key

#define Nk 4			// number of columns in a key
#define Nr 10			// number of rounds in encryption

typedef struct  {
	uint8_t expkey[4 * Nb * (Nr + 1)] ;
} aes128_ctx_t __attribute__ ((aligned (4)));

void aes128_enc (uint8_t *block, aes128_ctx_t* ctx);
void aes128_dec(uint8_t *block, aes128_ctx_t* ctx);
void aes128_init(const uint8_t *key, aes128_ctx_t* ctx);

#ifdef __cplusplus
}
#endif

#endif /* AES_H_ */
