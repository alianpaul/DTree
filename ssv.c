#include "ssv.h"
#include "utils.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFF_SIZE           32768
#define DEFAULT_DATA_SIZE   65536

//private
char*
skip_space(char* line_buff)
{
  char*  str_beg = line_buff;
  size_t str_len = strlen(line_buff); //str_len has been read
  
  for(size_t i = 0; i < str_len; ++i, ++str_beg)
    if(!isspace(line_buff[i])) break;
  return str_beg;
}

//private
char*
read_data_line(char* line_buff, FILE* file_stream)
{
  char*  str_p = line_buff; //points the right position in the line_buff.

  while(1)
    {
        if(fgets(line_buff, BUFF_SIZE, file_stream) == NULL)
	  {
	    str_p = NULL; //EOF
	    break;
	  }

	if(strlen(line_buff) > BUFF_SIZE - 2)
	  USER_ERROR("The line_buff is overflow");
  
	//skip beginning whitespace
	str_p = skip_space(line_buff);
  
	if(strlen(str_p) > 0)
	  break; //read something
    }

  return str_p;
}

void**
read_ssv_to_data(char* filename,
		 size_t* num_feat_p,
		 size_t* num_data_p,
		 SSVINFO* ssvinfo_p)
{
  FILE* file_p = NULL;

  if ( (file_p = fopen(filename, "r")) == NULL )
    USER_ERROR0("Can't find file %s", filename);

  char* line_buff = getmem(BUFF_SIZE);
  char* str_p     = line_buff;

  str_p = read_data_line(line_buff, file_p);
  while(str_p != NULL)
    {
      printf("%s", str_p);
      str_p = read_data_line(line_buff, file_p);
    }
  
  return NULL;
}

int main()
{
  SSVINFO ssvinfo;
  read_ssv_to_data("data/noisy10_test.ssv", NULL, NULL, &ssvinfo);
}
