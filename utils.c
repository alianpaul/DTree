#include "utils.h"

void* getmem(size_t bytes)
{

  void* ptr;
  if((ptr = malloc(bytes)) == (void*) NULL)
    USER_ERROR0("malloc %lu bytes", bytes);
  return ptr;  
}


