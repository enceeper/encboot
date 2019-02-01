/* macros.h

   Copyright (C) 2001, 2010 Niels Möller

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

#ifndef NETTLE_MACROS_H_INCLUDED
#define NETTLE_MACROS_H_INCLUDED

/* And the other, little-endian, byteorder */
#define LE_READ_UINT32(p)			\
(  (((uint32_t) (p)[3]) << 24)			\
 | (((uint32_t) (p)[2]) << 16)			\
 | (((uint32_t) (p)[1]) << 8)			\
 |  ((uint32_t) (p)[0]))

#define LE_WRITE_UINT32(p, i)			\
do {						\
  (p)[3] = ((i) >> 24) & 0xff;			\
  (p)[2] = ((i) >> 16) & 0xff;			\
  (p)[1] = ((i) >> 8) & 0xff;			\
  (p)[0] = (i) & 0xff;				\
} while(0)

/* Macro to make it easier to loop over several blocks. */
#define FOR_BLOCKS(length, dst, src, blocksize)	\
  assert( !((length) % (blocksize)));           \
  for (; (length); ((length) -= (blocksize),	\
		  (dst) += (blocksize),		\
		  (src) += (blocksize)) )

/* The masking of the right shift is needed to allow n == 0 (using
   just 32 - n and 64 - n results in undefined behaviour). Most uses
   of these macros use a constant and non-zero rotation count. */
#define ROTL32(n,x) (((x)<<(n)) | ((x)>>((-(n)&31))))

/* Requires that size > 0 */
#define INCREMENT(size, ctr)			\
  do {						\
    unsigned increment_i = (size) - 1;		\
    if (++(ctr)[increment_i] == 0)		\
      while (increment_i > 0			\
	     && ++(ctr)[--increment_i] == 0 )	\
	;					\
  } while (0)

#endif /* NETTLE_MACROS_H_INCLUDED */
