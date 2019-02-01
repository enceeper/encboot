//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Use the Nettle lib to perform AES256-CCM decryption
//
// Copyright (C) 2019 Vassilis Poursalidis (poursal@gmail.com)
//
// This program is free software: you can redistribute it and/or modify it under the terms of the
// GNU General Public License as published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
// even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License along with this program. If
// not, see <https://www.gnu.org/licenses/>.
//

#include "nettle/ccm.h"

int decryptccm_v1(unsigned char *iv, unsigned char *key, unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext)
{
	struct ccm_aes256_ctx ctx;
	ccm_aes256_set_key(&ctx, key);

	if ( ccm_aes256_decrypt_message(&ctx, 13, iv, 0, 0, 16, ciphertext_len, plaintext, ciphertext)==1 ) {
		return ciphertext_len;
	}
	else {
		return -1;
	}
}
