#include "bitarray.h"

int main()
{

  uchar* bitarray;
  size_t size_bit  = 71;
  size_t size_byte = (size_bit - 1) / 8 + 1;
  
  ALLOC_BITARRAY(bitarray, size_bit);

  printf("before\n");
  for(size_t i = 0; i < size_byte; ++i)
    {
      printf(BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(bitarray[i]));
    }
  
  for(size_t i = 0; i < 71; ++i)
    {
      SET_BITARRAY_ONE(bitarray, i);
    }

  printf("after one\n");
  for(size_t i = 0; i < size_byte; ++i)
    {
      printf(BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(bitarray[i]));
    }

  for(size_t i = 0; i < 71; i+=2)
    {
      SET_BITARRAY_ZERO(bitarray, i);
    }

  printf("after zero\n");
  for(size_t i = 0; i < size_byte; ++i)
    {
      printf(BYTE_TO_BINARY_PATTERN "\n", BYTE_TO_BINARY(bitarray[i]));
    }

  printf("check\n");
  for(size_t i = 0; i < 71; i+=2)
    {
      if(GET_BITARRAY(bitarray, i))
	USER_ERROR("Expected 0, but got 1");
    }

  printf("check\n");
  for(size_t i = 1; i < 71; i+=2)
    {
      if(!GET_BITARRAY(bitarray, i))
	USER_ERROR("Expected 1, but got 0");
    }

  

  
}
