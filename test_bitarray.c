#include "bitarray.h"

int main()
{
  uchar* array;
  ALLOC_BITARRAY(array, 0);
  printf("%d\n", array[0]);
  
  REALLOC_BITARRAY(array, 1);
  printf("%d\n", array[0]);
  
  REALLOC_BITARRAY(array, 8);
  printf("%d\n", array[0]);
  printf("%d\n", array[1]);
  
  REALLOC_BITARRAY(array, 9);
  printf("%d\n", array[0]);
  printf("%d\n", array[1]);
  
  FREE_BITARRAY(array);

  
  
}
