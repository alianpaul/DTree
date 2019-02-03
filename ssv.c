#include "ssv.h"
#include "utils.h"
#include "bitarray.h"


#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <search.h>

//#define NDEBUG
#include <assert.h>


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
read_ssv_feat_names(char* buff_p,
		    SSVINFO* ssvinfo_p)
{
  ssvinfo_p->feat_names = (char**) getmem(ssvinfo_p->num_feats *
					  sizeof(char*));
  for(size_t i = 0; i < ssvinfo_p->num_feats; ++i)
    {
      char* feat_name_src = next_word(&buff_p);
      ssvinfo_p->feat_names[i] = (char*) getmem(strlen(feat_name_src) + 1);
      strcpy(ssvinfo_p->feat_names[i], feat_name_src);
    }
  /*
  for(size_t i = 0; i < ssvinfo_p->num_feats; ++i)
    {
      printf("%s\n", ssvinfo_p->feat_names[i]);
    }
  */
}

void
read_ssv_feat_types(char* buff_p,
		    SSVINFO* ssvinfo_p)
{
  char* feat_type_src = next_word(&buff_p);
  ssvinfo_p->feat_types = (char*) getmem(strlen(feat_type_src) + 1);
  strcpy(ssvinfo_p->feat_types, feat_type_src);

  if(strlen(ssvinfo_p->feat_types) != ssvinfo_p->num_feats)
    USER_ERROR("num_feats doesn't match");

  /*
  printf("%s %lu\n", ssvinfo_p->feat_types, strlen(ssvinfo_p->feat_types));
  */
}

void
read_ssv_data(char* buff_p,
	      SSVINFO* ssvinfo_p,
	      size_t i_data)
{
  for(size_t i_feat = 0; i_feat < ssvinfo_p->num_feats; ++i_feat)
    {
      char*  attr_name     = next_word(&buff_p);
      size_t attr_name_len = 0;
      char   feat_type     = ssvinfo_p->feat_types[i_feat];
      char*  feat_name     = ssvinfo_p->feat_names[i_feat];
      size_t feat_name_len = strlen(feat_name);
	  
      switch(feat_type)
	{
	case 'b':
	  {
	    unsigned char attr_val_b = *attr_name - '0';
	    write_attrib_b(ssvinfo_p->data, i_data, i_feat, attr_val_b);
	    assert(attr_val_b ==
		   read_attrib_b(ssvinfo_p->data, i_data, i_feat));
	  }
	  break;
	case 'd':
	  {
	    int           attr_val_d = -1;
	    attr_name_len = strlen(attr_name);

	    //Form the key
	    ENTRY* e_f = (ENTRY*) getmem(sizeof(ENTRY));
	    ENTRY* e_r = NULL;
	    e_f->key   = (char*)  getmem(attr_name_len +
					 feat_name_len + 1);
	    strcpy(e_f->key, feat_name);
	    strncpy(&(e_f->key[feat_name_len]), attr_name, attr_name_len);

	    // Test if the key exists. If not, add it to
	    // ssvinfo_p->num_discrete_vals
	    // ssvinfo_p->discrete_vals

	    if( (e_r = hsearch(*e_f, FIND)) == NULL)
	      {
		//Attribute feature has not been seen before
		//1. update # of discrete_vals of this feature
		attr_val_d   = ssvinfo_p->num_discrete_vals[i_feat]++;
		e_f->data    = (void*) getmem(sizeof(int));
		*((int*)e_f->data) = attr_val_d;
		if(hsearch(*e_f, ENTER) == NULL)
		  USER_ERROR("hash table insert error");

		//2. update discrete_vals
		char*  attr_name_add = (char*) getmem(attr_name_len + 1);
		strcpy(attr_name_add, attr_name);

		//attr_val_d is also the attribute values that we already has
		if(attr_val_d == 0)
		  {
		    //not allocate yet, malloc the first one
		    ssvinfo_p->discrete_vals[i_feat] =
		      (char**) malloc(sizeof(char*));
		  }
		else
		  {
		    assert(attr_val_d > 0);
		    //ready have, realloc
		    ssvinfo_p->discrete_vals[i_feat] =
		      (char**) realloc(ssvinfo_p->discrete_vals[i_feat],
				       sizeof(char*)
				       *
				       ssvinfo_p->num_discrete_vals[i_feat]);
		  }

		ssvinfo_p->discrete_vals[i_feat][attr_val_d] = attr_name_add;
	      }
	    else
	      {
		//has been seen already, read the val directly
		attr_val_d = *((int*)e_r->data);

		char* store_attr_name =
		  ssvinfo_p->discrete_vals[i_feat][attr_val_d];
		assert(strcmp(store_attr_name, attr_name) == 0);
		
	      }
	    write_attrib_i(ssvinfo_p->data, i_data, i_feat, attr_val_d);
	    assert(attr_val_d ==
		   read_attrib_i(ssvinfo_p->data, i_data, i_feat));
	  }
	  break;
	case 'c':
	  //printf("c %s\n", feat_val);
	  break;
	}
    }
}

void
read_ssv(char* filename_in,
	 SSVINFO* ssvinfo_p)
{
  FILE*  file_p = NULL;
  size_t num_feats = 0;
  size_t num_datas = 0;
  size_t num_datas_alloc = 0;

  if ( (file_p = fopen(filename_in, "r")) == NULL )
    USER_ERROR0("Can't find file %s", filename_in);

  char* line_buff = getmem(BUFF_SIZE);
  char* buff_p    = line_buff;

  //num of features and num of datapoints
  buff_p    = read_data_line(line_buff, file_p);
  num_feats = atoi(next_word(&buff_p));
  num_datas = atoi(next_word(&buff_p));

  ssvinfo_p->num_feats = num_feats;

  if(num_datas == 0) num_datas_alloc = DEFAULT_DATA_SIZE;
  else num_datas_alloc = num_datas;

  //read the features name into ssvinfo_p;
  buff_p = read_data_line(line_buff, file_p);
  read_ssv_feat_names(buff_p, ssvinfo_p);

  //read the type of each feature into ssvinfo_p
  buff_p = read_data_line(line_buff, file_p);
  read_ssv_feat_types(buff_p, ssvinfo_p);
  
  //Init read data
  ssvinfo_p->data              = (void**)  getmem(num_feats * sizeof(void*));
  ssvinfo_p->num_discrete_vals = (int*)    getmem(num_feats * sizeof(int));
  ssvinfo_p->discrete_vals     = (char***) getmem(num_feats * sizeof(char**));
  
  //init data
  for(size_t i_feat = 0; i_feat < num_feats; ++i_feat)
    {
      char feat_type = ssvinfo_p->feat_types[i_feat];
      switch(ssvinfo_p->feat_types[i_feat])
	{
	case 'b': /*Binary   type:  Use 1 bit to represent*/
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

  hcreate(num_datas_alloc * num_feats); //record the value of features' all attr
  size_t num_data_read = 0;
  for(size_t i_data = 0; i_data < num_datas_alloc; ++i_data)
    {
      if((buff_p = read_data_line(line_buff, file_p)) == NULL) break;

      ++num_data_read;
      //process current data
      //printf("%s\n", buff_p);
      read_ssv_data(buff_p, ssvinfo_p, i_data);
    }

  ssvinfo_p->num_datas = num_data_read;
}

void
write_ssv(char* filename_out,
	  SSVINFO* ssvinfo_p)
{
  FILE*  file_p = NULL;
  if ( (file_p = fopen(filename_out, "w")) == NULL )
    USER_ERROR0("Can't find file %s", filename_out);

  //num of features and num of datas
  fprintf(file_p, "%zu %zu\n", ssvinfo_p->num_feats, ssvinfo_p->num_datas);

  //feature names
  for(size_t i_feat = 0; i_feat < ssvinfo_p->num_feats; ++i_feat)
    {
      fprintf(file_p, "%s", ssvinfo_p->feat_names[i_feat]);
      if(i_feat == (ssvinfo_p->num_feats - 1))
	fprintf(file_p, "\n");
      else
	fprintf(file_p, " ");
    }

  //feature types
  fprintf(file_p, "%s\n", ssvinfo_p->feat_types);

  //data
  for(size_t i_data = 0; i_data < ssvinfo_p->num_datas; ++i_data)
    {
      for(size_t i_feat = 0; i_feat < ssvinfo_p->num_feats; ++i_feat)
	{
	  void* feat_examples_p = ssvinfo_p->data[i_feat];
	  if(ssvinfo_p->feat_types[i_feat] == 'b')
	    {
	      if(GET_BITARRAY(feat_examples_p, i_data))
		fprintf(file_p, "%d", 1);
	      else
		fprintf(file_p, "%d", 0);
	    }
	  else if(ssvinfo_p->feat_types[i_feat] == 'd')
	    {
	      int i_discrete = ((int*)feat_examples_p)[i_data];
	      fprintf(file_p, "%s", ssvinfo_p->discrete_vals[i_feat][i_discrete]);
	    }

	  if(i_feat == (ssvinfo_p->num_feats - 1))
	    fprintf(file_p, "\n");
	  else
	    fprintf(file_p, " ");
	}
    }
}

/*
int main()
{
  SSVINFO ssvinfo;
  read_ssv ("data/noisy10_train.ssv", &ssvinfo);
  write_ssv("train.out",              &ssvinfo);
}
*/
