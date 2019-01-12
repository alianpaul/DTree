#include "ssv.h"
#include "utils.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFF_SIZE           32768
#define DEFAULT_DATA_SIZE   65536

//private
/*
 * return a pointer to the next word. and update str_pp
 */
char*
next_word(char** str_pp)
{
  size_t str_len  = strlen(*str_pp); //str_len has been read  
  size_t i        = 0;
  
  //move to the beginning of the next word
  for(i = 0; i < str_len; ++i, ++(*str_pp))
    if(!isspace(**str_pp)) break;
  
  char* next_word_p = *str_pp;

  //move to the ending of the next word
  for(     ; i < str_len; ++i, ++(*str_pp))
    if( isspace(**str_pp)) break;

  **str_pp = '\0';
  (*str_pp)++;

  return next_word_p;
}

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
  char* buff_p     = line_buff;

  //num of features and num of datapoints
  buff_p = read_data_line(line_buff, file_p);
  size_t num_feats = atoi(next_word(&buff_p));
  size_t num_datas = atoi(next_word(&buff_p));

  //read the features name into ssvinfo_p;
  buff_p = read_data_line(line_buff, file_p);
  ssvinfo_p->feat_names = (char**) getmem(num_feats * sizeof(char*));
  
  for(size_t i = 0; i < num_feats; ++i)
    {
      char* feat_name_src = next_word(&buff_p);
      ssvinfo_p->feat_names[i] = (char*) getmem(strlen(feat_name_src) + 1);
      //+1 for null char
      strcpy(ssvinfo_p->feat_names[i], feat_name_src);
    }
  
  while(buff_p != NULL)
    {
      break;
      buff_p = read_data_line(line_buff, file_p);
    }
  
  return NULL;
}

int main()
{
  SSVINFO ssvinfo;
  read_ssv_to_data("data/noisy10_train.ssv", NULL, NULL, &ssvinfo);
}
