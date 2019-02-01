//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Our AES256-CCM decrypt implementation
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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "decryptor.h"
#include "codec/hex.h"

// Code from: https://github.com/google/fscryptctl
/* We either use dedicated memory clearing functions or volatile dereference. */
void secure_wipe(uint8_t *v, uint32_t n) {
#if defined memset_s
	memset_s(v, n, 0, n);
#elif defined explicit_bzero
	explicit_bzero(v, n);
#else
	volatile uint8_t *p = v;
	while (n--) *p++ = 0;
#endif
}

int main(int argc, char **argv)
{
	unsigned char *iv;
	size_t ivlen;
	unsigned char *key;
	size_t keylen;
	unsigned char *ciphertext;
	size_t ciphertextlen;
	unsigned char *plaintext;
	size_t plaintextlen;

	if (argc != 4) {
		printf("ERROR: missing argument. Usage: decryptor <hex IV> <hex key> <hex ciphertext>\n");
		return -1;
	}

	ivlen = hex2bin(argv[1], &iv);
	if ( ivlen==0 ) {
		printf("ERROR: Not a valid HEX input value for IV.\n");
		return -2;
	}
	keylen = hex2bin(argv[2], &key);
	if ( keylen==0 ) {
		printf("ERROR: Not a valid HEX input value for the decryption key.\n");
		return -2;
	}
	ciphertextlen = hex2bin(argv[3], &ciphertext);
	if ( ciphertextlen==0 ) {
		printf("ERROR: Not a valid HEX input value for IV.\n");
		return -2;
	}

	// Allocate memory for plaintext
	ciphertextlen -= 16;
	plaintext = malloc(ciphertextlen);
	plaintextlen = decryptccm_v1(iv, key, ciphertext, ciphertextlen, plaintext);

	if ( plaintextlen!=-1 ) {
		printf("%s\n", bin2hex(plaintext, plaintextlen));
	}

	secure_wipe(key, keylen);
	secure_wipe(plaintext, ciphertextlen);
	free(plaintext);

	if ( plaintextlen!=-1 ) {
		return 0;
	}
	else {
		return -3;
	}
}
