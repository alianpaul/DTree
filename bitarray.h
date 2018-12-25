#ifndef DECISION_TREE_BITARRAY_H
#define DECISION_TREE_BITARRAY_H 1

#include <stdlib.h>

#include "utils.h"

/* Bit array ops:
 * ALLOC_BITARRAY   (bitarray, size)
 * REALLOC_BITARRAY (bitarray, size)
 * FREE_BITARRAY    (bitarray)
 * SET_ONE  (bitarray, idx)
 * SET_ZERO (bitarray, idx)
 * SET_ONE_RANGE  (bitarray, start, end)
 * SET_ZERO_RANGE (bitarray, start, end)
*/

#ifndef uchar
#define uchar unsigned char
#endif

#define ALLOC_BITARRAY(bitarray, size) \
  (bitarray) = (uchar*) malloc(((size - 1)/8 + 1) * sizeof(uchar))


#define FREE_BITARRAY(bitarray) \
  free((bitarray))

#define REALLOC_BITARRAY(bitarray, new_size) \
  (bitarray) = (uchar*) realloc((bitarray),  \
				(((new_size) - 1)/8 + 1) * sizeof(uchar))



#endif
