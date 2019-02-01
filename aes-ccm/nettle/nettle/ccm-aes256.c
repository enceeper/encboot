/* ccm-aes256.c

   Counter with CBC-MAC mode using AES256 as the underlying cipher.

   Copyright (C) 2014 Exegin Technologies Limited
   Copyright (C) 2014 Owen Kirby

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

#if HAVE_CONFIG_H
# include "config.h"
#endif

#include <assert.h>

#include "aes.h"
#include "ccm.h"

void
ccm_aes256_set_key(struct ccm_aes256_ctx *ctx, const uint8_t *key)
{
  aes256_set_encrypt_key(&ctx->cipher, key);
}

int
ccm_aes256_decrypt_message(struct ccm_aes256_ctx *ctx,
			   size_t nlength, const uint8_t *nonce,
			   size_t alength, const uint8_t *adata,
			   size_t tlength,
			   size_t mlength, uint8_t *dst, const uint8_t *src)
{
  return ccm_decrypt_message(&ctx->cipher, (nettle_cipher_func *) aes256_encrypt,
			     nlength, nonce, alength, adata,
			     tlength, mlength, dst, src);
}
