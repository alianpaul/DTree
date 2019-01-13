#include "ssv.h"
#include "utils.h"
#include "bitarray.h"

#include <stdio.h>
#include <ctype.h>
#include <string.h>

#define BUFF_SIZE           32768
#define DEFAULT_DATA_SIZE   65536

/* Read a binary (0/1) value. */
unsigned char read_attrib_b(void **data, int example, int feature)
{
  return GET_BITARRAY(data[feature], example);
}
/* Write a binary (0/1) value. */
void write_attrib_b(void **data, int example, int feature, unsigned char val)
{
  if(val == 1)
    SET_BITARRAY_ONE (data[feature], example);
  else if(val == 0)
    SET_BITARRAY_ZERO(data[feature], example);
  else
    USER_ERROR0("Can't set value %c to bitarray", val);
}

/* Read an integer. */
int read_attrib_i(void **data, int example, int feature)
{
  return ((int *) data[feature])[example];
}
/* Write an integer. */
void write_attrib_i(void **data, int example, int feature, int val)
{
  ((int *) data[feature])[example] = val;
}

/* Read a double. */
double read_attrib_c(void **data, int example, int feature)
{
  return ((double *) data[feature])[example];
}
/* Write a double. */
void write_attrib_c(void **data, int example, int feature, double val)
{
  ((double *) data[feature])[example] = val;
}



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

  //change all trailing space character to null character
  for(int i = str_len - 1; i >= 0; line_buff[i] = '\0', --i)
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
	
	//skip beginning and trailing whitespace
	str_p = skip_space(line_buff);
	
	if(strlen(str_p) > 0)
	    break; //read something
    }

  return str_p;
}

void
read_ssv_to_data(char* filename,
		 SSVINFO* ssvinfo_p)
{
  FILE*  file_p = NULL;
  size_t num_feats = 0;
  size_t num_datas = 0;
  size_t num_datas_alloc = 0;

  if ( (file_p = fopen(filename, "r")) == NULL )
    USER_ERROR0("Can't find file %s", filename);

  char* line_buff = getmem(BUFF_SIZE);
  char* buff_p    = line_buff;

  //num of features and num of datapoints
  buff_p    = read_data_line(line_buff, file_p);
  num_feats = atoi(next_word(&buff_p));
  num_datas = atoi(next_word(&buff_p));

  if(num_datas == 0) num_datas_alloc = DEFAULT_DATA_SIZE;
  else num_datas_alloc = num_datas;

  //read the features name into ssvinfo_p;
  buff_p = read_data_line(line_buff, file_p);
  ssvinfo_p->feat_names = (char**) getmem(num_feats * sizeof(char*));
  for(size_t i = 0; i < num_feats; ++i)
    {
      char* feat_name_src = next_word(&buff_p);
      ssvinfo_p->feat_names[i] = (char*) getmem(strlen(feat_name_src) + 1);
      strcpy(ssvinfo_p->feat_names[i], feat_name_src);
    }
  for(size_t i = 0; i < num_feats; ++i)
    {
      printf("%s\n", ssvinfo_p->feat_names[i]);
    }

  //read the type of each feature into ssvinfo_p
  buff_p = read_data_line(line_buff, file_p);
  char* feat_type_src = next_word(&buff_p);
  ssvinfo_p->feat_types = (char*) getmem(strlen(feat_type_src) + 1);
  strcpy(ssvinfo_p->feat_types, feat_type_src);
  printf("%s %lu\n", ssvinfo_p->feat_types, strlen(ssvinfo_p->feat_types));
  if(strlen(ssvinfo_p->feat_types) != num_feats)
    USER_ERROR("num_feats doesn't match");

  //Init read data
  ssvinfo_p->data              = (void**)  getmem(num_feats * sizeof(void*));
  ssvinfo_p->num_discrete_vals = (int*)    getmem(num_feats * sizeof(int));
  ssvinfo_p->discrete_vals     = (char***) getmem(num_feats * sizeof(char**));
  //init data
  for(size_t i_feat = 0; i_feat < num_feats; ++i_feat)
    {
      char feat_type = ssvinfo_p->feat_types[i_feat];
      printf("%c\n", feat_type);
      switch(ssvinfo_p->feat_types[i_feat])
	{
	case 'b': /*Binary type:  Use 1 bit to represent*/
	  ALLOC_BITARRAY(ssvinfo_p->data[i_feat],
			 num_datas_alloc);
	  break;
	case 'd': /*Discrete type: Use int to represent*/
	  ssvinfo_p->data[i_feat]
	    = (int*)    getmem(num_datas_alloc * sizeof(int   ));
	  break;
	case 'c': /*Continious type: Use double to represent*/
	  ssvinfo_p->data[i_feat]
	    = (double*) getmem(num_datas_alloc * sizeof(double)); 
	  break;
	default :
	  USER_ERROR0("Unknown type %c. Only accept b,d,c", feat_type);
	  break;
	}
    }
  //init num_discrete_vals
  memset(ssvinfo_p->num_discrete_vals, 0, num_feats);
  memset(ssvinfo_p->discrete_vals,     0, num_feats * sizeof(char**));

  size_t num_data_read = 0;
  for(size_t i_data = 0; i_data < num_datas_alloc; ++i_data)
    {
      if((buff_p = read_data_line(line_buff, file_p)) == NULL) break;

      ++num_data_read;
      //process current data
      printf("%s\n", buff_p);

      for(size_t i_feat = 0; i_feat < num_feats; ++i_feat)
	{
	  char* feat_val  = next_word(&buff_p);
	  char  feat_type = ssvinfo_p->feat_types[i_feat];

	  switch(feat_type)
	    {
	    case 'b':
	      printf("b %s\n", feat_val);
	      unsigned char val= *feat_val - '0';
	      write_attrib_b(ssvinfo_p->data, i_data, i_feat, val);
	      //printf("CHECK b %d\n", read_attrib_b(ssvinfo_p->data, i_data, i_feat));
	      break;
	    case 'd':
	      //printf("d %s\n", feat_val);
	      //size_t attr_name_len = strlen(feat_val);
	      break;
	    case 'c':
	      //printf("c %s\n", feat_val);
	      break;
	    }
	}
      if(num_data_read == 20)
	break;
    }
}

int main()
{
  SSVINFO ssvinfo;
  read_ssv_to_data("data/noisy10_train.ssv", &ssvinfo);
}
