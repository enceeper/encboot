//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Use the OpenSSL lib to perform AES256-CCM decryption
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
// Code taken from: https://wiki.openssl.org/index.php/EVP_Authenticated_Encryption_and_Decryption

#include <openssl/evp.h>
#include <openssl/err.h>

void handleErrors(void)
{
	ERR_print_errors_fp(stderr);
	abort();
}

int decryptccm_v1(unsigned char *iv, unsigned char *key, unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext)
{
	EVP_CIPHER_CTX *ctx;
	int len;
	int plaintext_len;
	int ret;

	/* Create and initialise the context */
	if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

	/* Initialise the decryption operation. */
	if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_ccm(), NULL, NULL, NULL))
		handleErrors();

	/* Setting IV len to 13. */
	if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_IVLEN, 13, NULL))
		handleErrors();

	/* Set expected tag value. */
	if(1 != EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_CCM_SET_TAG, 16, ciphertext+ciphertext_len))
		handleErrors();

	/* Initialise key and IV */
	if(1 != EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) handleErrors();

	/* Provide the total ciphertext length
	 */
	if(1 != EVP_DecryptUpdate(ctx, NULL, &len, NULL, ciphertext_len))
		handleErrors();

	/* Provide the message to be decrypted, and obtain the plaintext output.
	 * EVP_DecryptUpdate can be called multiple times if necessary
	 */
	ret = EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);

	plaintext_len = len;

	/* Clean up */
	EVP_CIPHER_CTX_free(ctx);

	if(ret > 0)
	{
		/* Success */
		return plaintext_len;
	}
	else
	{
		/* Verify failed */
		return -1;
	}
}
