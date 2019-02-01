/* nettle-types.h

   Copyright (C) 2005, 2014 Niels Möller

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

#ifndef NETTLE_TYPES_H
#define NETTLE_TYPES_H

/* For size_t */
#include <stddef.h>

/* Pretend these types always exists. Nettle doesn't use them. */
#define _STDINT_HAVE_INT_FAST32_T 1
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* An aligned 16-byte block. */
union nettle_block16
{
  uint8_t b[16];
  unsigned long w[16 / sizeof(unsigned long)];
};

/* For block ciphers, const context. */
typedef void nettle_cipher_func(const void *ctx,
				size_t length, uint8_t *dst,
				const uint8_t *src);

#ifdef __cplusplus
}
#endif

#endif /* NETTLE_TYPES_H */
