//
// SPDX-License-Identifier: GPL-3.0-or-later
//
// Decryptor definition for v1 to be pluggable (either openssl via shared library or the included nettle code)
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

#ifndef _DECRYPTOR_H_
#define _DECRYPTOR_H_

#ifdef __cplusplus
extern "C" {
#endif

int decryptccm_v1(unsigned char *iv, unsigned char *key, unsigned char *ciphertext, int ciphertext_len, unsigned char *plaintext);

#ifdef __cplusplus
}
#endif

#endif //_DECRYPTOR_H_
