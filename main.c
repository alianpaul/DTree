#include "bitarray.h"

#include <stdio.h>

void print_bitarray(uchar* bitarray, size_t size)
{
  printf("Contents of bit array\n");
  size = (size - 1) / 8 + 1;
  for(size_t i = 0 ; i < size; ++i)
    {
      printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(*(bitarray + i)));
    }
  
}

int main()
{
  /*
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY((uchar)0));
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY((uchar)1));
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY((uchar)2));
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY((uchar)4));
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY((uchar)8));
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY((uchar)16));
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY((uchar)32));
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY((uchar)64));
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY((uchar)128));
  printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY((uchar)255));
  */

  uchar* bitarray = NULL;
  size_t size     = 18;  //1bit
  ALLOC_BITARRAY(bitarray, size);

  for(size_t i = 0; i < size; ++i)
    {
      SET_BITARRAY_ONE(bitarray, i);
      //print_bitarray(bitarray, size);
    }

  for(size_t i = 0; i < size; ++i)
    {
      SET_BITARRAY_ZERO(bitarray, i);
      print_bitarray(bitarray, size);
    }
  
  FREE_BITARRAY(bitarray);
}
