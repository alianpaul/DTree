#include "ssv.h"
#include "utils.h"

#include <stdio.h>

void** read_ssv_file(char* filename,
		     size_t* num_feat_p,
		     size_t* num_data_p,
		     SSVINFO* ssvinfo_p)
{
  FILE * file_p = NULL;

  if ( (file_p = fopen(filename, "r")) == NULL )
    USER_ERROR0("Can't find file %s", filename);

  return NULL;
}

int main()
{
  SSVINFO ssvinfo;
  read_ssv_file("ssv.h", NULL, NULL, &ssvinfo);
  read_ssv_file("ssv.hx", NULL, NULL, &ssvinfo);
}
