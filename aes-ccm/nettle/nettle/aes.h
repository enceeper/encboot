/* aes.h

   The aes/rijndael block cipher.

   Copyright (C) 2001, 2013 Niels Möller

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
 
#ifndef NETTLE_AES_H_INCLUDED
#define NETTLE_AES_H_INCLUDED

#include "nettle-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Name mangling */
#define aes256_set_encrypt_key nettle_aes256_set_encrypt_key
#define aes256_encrypt nettle_aes256_encrypt

#define AES_BLOCK_SIZE 16

#define AES256_KEY_SIZE 32
#define _AES256_ROUNDS 14

/* Variable key size between 128 and 256 bits. But the only valid
 * values are 16 (128 bits), 24 (192 bits) and 32 (256 bits). */
#define AES_MIN_KEY_SIZE AES256_KEY_SIZE
#define AES_MAX_KEY_SIZE AES256_KEY_SIZE

/* Older nettle-2.7 interface */

#define AES_KEY_SIZE 32

struct aes256_ctx
{
  uint32_t keys[4 * (_AES256_ROUNDS + 1)];
};

void
aes256_set_encrypt_key(struct aes256_ctx *ctx, const uint8_t *key);
void
aes256_encrypt(const struct aes256_ctx *ctx,
	       size_t length, uint8_t *dst,
	       const uint8_t *src);

#ifdef __cplusplus
}
#endif

#endif /* NETTLE_AES_H_INCLUDED */
