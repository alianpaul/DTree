#include "utils.h"

void* getmem(size_t bytes)
{

  void* ptr;
  if((ptr = malloc(bytes)) == (void*) NULL)
    USER_ERROR0("malloc %lu bytes", bytes);
  return ptr;
  
}

int main()
{

  void* ptr_0 = getmem(16);
  //USER_EEROR2("Some error happened, %d %d %f", 8, 9, 1.0);
  USER_ERROR0("Error, %p", ptr_0);
}
