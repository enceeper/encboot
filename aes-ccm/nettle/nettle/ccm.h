/* ccm.h

   Counter with CBC-MAC mode, specified by NIST,
   http://csrc.nist.gov/publications/nistpubs/800-38C/SP800-38C_updated-July20_2007.pdf

   Copyright (C) 2014 Exegin Technologies Limited
   Copyright (C) 2014 Owen Kirby

   Contributed to GNU Nettle by Owen Kirby

   This file is part of GNU Nettle.

   GNU Nettle is free software: you can redistribute it and/or
   modify it under the terms of either:

     * the GNU Lesser General Public License as published by the Free
       Software Foundation; either version 3 of the License, or (at your
       option) any later version.

   or

     * the GNU General Public License as published by the Free
       Software Foundation; either version 2 of the License, or (at your
       option) any later version.

   or both in parallel, as here.

   GNU Nettle is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received copies of the GNU General Public License and
   the GNU Lesser General Public License along with this program.  If
   not, see http://www.gnu.org/licenses/.
*/

/* NIST SP800-38C doesn't specify the particular formatting and
 * counter generation algorithm for CCM, but it does include an
 * example algorithm. This example has become the de-factor standard,
 * and has been adopted by both the IETF and IEEE across a wide
 * variety of protocols.
 */

#ifndef NETTLE_CCM_H_INCLUDED
#define NETTLE_CCM_H_INCLUDED

#include "aes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Name mangling */
#define ccm_set_nonce nettle_ccm_set_nonce
#define ccm_update nettle_ccm_update
#define ccm_decrypt nettle_ccm_decrypt
#define ccm_digest nettle_ccm_digest
#define ccm_decrypt_message nettle_ccm_decrypt_message

#define ccm_aes256_set_key nettle_ccm_aes256_set_key
#define ccm_aes256_decrypt_message nettle_ccm_aes256_decrypt_message

/* For CCM, the block size of the block cipher shall be 128 bits. */
#define CCM_BLOCK_SIZE  16
#define CCM_DIGEST_SIZE 16
#define CCM_MIN_NONCE_SIZE 7
#define CCM_MAX_NONCE_SIZE 14

/* Per-message state */
struct ccm_ctx {
  union nettle_block16 ctr;     /* Counter for CTR encryption. */
  union nettle_block16 tag;     /* CBC-MAC message tag. */
  /* Length of data processed by the CBC-MAC modulus the block size */
  unsigned int blength;
};

/*
 * CCM mode requires the adata and message lengths when building the IV, which
 * prevents streaming processing and it incompatible with the AEAD API.
 */
void
ccm_set_nonce(struct ccm_ctx *ctx, const void *cipher, nettle_cipher_func *f,
	      size_t noncelen, const uint8_t *nonce,
	      size_t authlen, size_t msglen, size_t taglen);

void
ccm_update(struct ccm_ctx *ctx, const void *cipher, nettle_cipher_func *f,
	    size_t length, const uint8_t *data);

void
ccm_decrypt(struct ccm_ctx *ctx, const void *cipher, nettle_cipher_func *f,
	    size_t length, uint8_t *dst, const uint8_t *src);

void
ccm_digest(struct ccm_ctx *ctx, const void *cipher, nettle_cipher_func *f,
	   size_t length, uint8_t *digest);

/*
 * The decryption function will write the plaintext to dst and parse the digest
 * from the final tlength bytes of the ciphertext. If the digest matched the
 * value computed during decryption then this will return 1, or it will return
 * 0 if the digest was invalid.
 */
int
ccm_decrypt_message(const void *cipher, nettle_cipher_func *f,
		    size_t nlength, const uint8_t *nonce,
		    size_t alength, const uint8_t *adata,
		    size_t tlength,
		    size_t mlength, uint8_t *dst, const uint8_t *src);

/* CCM Mode with AES-256 */
struct ccm_aes256_ctx {
    struct ccm_ctx      ccm;
    struct aes256_ctx   cipher;
};

void
ccm_aes256_set_key(struct ccm_aes256_ctx *ctx, const uint8_t *key);

int
ccm_aes256_decrypt_message(struct ccm_aes256_ctx *ctx,
			   size_t nlength, const uint8_t *nonce,
			   size_t alength, const uint8_t *adata,
			   size_t tlength,
			   size_t mlength, uint8_t *dst, const uint8_t *src);

#ifdef __cplusplus
}
#endif

#endif /* NETTLE_CCM_H_INCLUDED */
