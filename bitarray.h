#ifndef DECISION_TREE_BITARRAY_H
#define DECISION_TREE_BITARRAY_H 1

#include <stdlib.h>

#include "utils.h"

/* Bit array ops:
 * ALLOC_BITARRAY      (bitarray, size)
 * REALLOC_BITARRAY    (bitarray, size)
 * FREE_BITARRAY       (bitarray)
 * SET_BITARRAY_ONE    (bitarray, idx)
 * SET_BITARRAY_ZERO   (bitarray, idx)
*/

#ifndef uchar
#define uchar unsigned char
#endif

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c \n"
#define BYTE_TO_BINARY(byte)   \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0') 

#define ALLOC_BITARRAY(bitarray, size) \
  (bitarray) = (uchar*) malloc(((size - 1)/8 + 1) * sizeof(uchar))

#define FREE_BITARRAY(bitarray) \
  free((bitarray))

#define REALLOC_BITARRAY(bitarray, new_size) \
  (bitarray) = (uchar*) realloc((bitarray),  \
				(((new_size) - 1)/8 + 1) * sizeof(uchar))

#define SET_BITARRAY_ONE(bitarray, idx)	\
  ((uchar*)bitarray)[(idx)/8] |= 0x80 >> ((idx) % 8)

#define SET_BITARRAY_ZERO(bitarray, idx) \
  ((uchar*)bitarray)[(idx)/8] &= ~(0x80 >> ((idx) % 8))

#define GET_BITARRAY(bitarray, idx) \
  (((uchar*)bitarray)[(idx)/8] & (0x80 >> ((idx) % 8)))

#endif
