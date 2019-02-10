#include "entropy.h"
#include "utils.h"

#include <math.h>
#include <string.h>

/*
 */
double entropy_b(size_t num_pos, size_t num_neg)
{

  if(num_pos == 0 || num_neg == 0)
    return 0.;
  
  double prob_pos = ((double)num_pos) / (double) (num_pos + num_neg);
  double prob_neg = ((double)num_neg) / (double) (num_pos + num_neg);

  return -prob_pos * log2(prob_pos) -prob_neg * log2(prob_neg);
}


/*
 * Predict on the first attribute(binary)
 */
void   count_examples(SSVINFO* ssvinfo_p,
		      uchar*   member_data_mask,
		      size_t   num_member_data,
		      size_t*  num_pos, size_t* num_neg)
{
  *num_pos = 0;
  *num_neg = 0;

  for(size_t i_data = 0; i_data < ssvinfo_p->num_datas; ++i_data)
    {
      //inference on the first binary attribute
      if(member_data_mask != NULL && GET_BITARRAY(member_data_mask, i_data) == 0)
	continue;

      //The first feature is the feature to predict
      if(read_attrib_b(ssvinfo_p->data, i_data, 0))
	++(*num_pos);
      else
	++(*num_neg);
    }
}

double entropy_data(SSVINFO* ssvinfo_p,
		    uchar*   member_data_mask,
		    size_t   num_member_data)
{
  size_t num_pos = 0, num_neg = 0;
  
  if (num_member_data == 0) return 0.0;
  
  count_examples(ssvinfo_p, member_data_mask, num_member_data, &num_pos, &num_neg);

  //printf("%zu %zu\n", num_pos, num_neg);

  return entropy_b(num_pos, num_neg);
}


/*Conditional entropy with discrete feature which indicated by i_feat.
 */
double conditional_entropy_data_d(SSVINFO* ssvinfo_p,
				  uchar* member_data_mask,
				  size_t num_member_data,
				  size_t i_feat)
{

  double conditional_entropy = 0.0;
  
  int    num_feat_val         = ssvinfo_p->num_discrete_vals[i_feat];
  size_t num_data             = ssvinfo_p->num_datas;
  void** data                 = ssvinfo_p->data;
  
  uchar* feat_val_member_mask = NULL;
  ALLOC_BITARRAY(feat_val_member_mask, ssvinfo_p->num_datas);
  size_t num_feat_val_member_data = 0;
  
  for(int feat_val = 0; feat_val < num_feat_val; ++feat_val)
    {
      num_feat_val_member_data = 0;
      SET_BITARRAY_RANGE_ZERO(feat_val_member_mask, 0, num_data);
      
      for(size_t i_data = 0; i_data < num_data; ++i_data)
	{

	  //Operate on all the data && 
	  if(member_data_mask != NULL && GET_BITARRAY(member_data_mask, i_data) == 0)
	    continue;

	  //example marked by member_data_mask
	  if(read_attrib_i(data, i_data, i_feat) == feat_val)
	    {
	      ++num_feat_val_member_data;
	      SET_BITARRAY_ONE(feat_val_member_mask, i_data);
	    }
	}

      /*
      printf("%s %s  val:%d cnt:%zu\n",
	     ssvinfo_p->feat_names[i_feat],
	     ssvinfo_p->discrete_vals[i_feat][feat_val],
	     feat_val,
	     num_feat_val_member_data);

      for(size_t i_byte = 0; i_byte < 2; ++i_byte)
	{
	  printf(BYTE_TO_BINARY_PATTERN,
		 BYTE_TO_BINARY(feat_val_member_mask[i_byte]));
	}
      */
      
      double specific_entropy = entropy_data(ssvinfo_p,
					     feat_val_member_mask,
					     num_feat_val_member_data);
      //printf("specific entropy %lf\n", specific_entropy);
      conditional_entropy += specific_entropy * num_feat_val_member_data;
    }
  conditional_entropy /= num_member_data;
  free(feat_val_member_mask);
  
  return conditional_entropy;
}


/*Conditional entropy with continuous feature
 */
double conditional_entropy_data_c(SSVINFO* ssvinfo_p,
				  uchar* member_data_mask,
				  size_t num_member_data,
				  size_t i_feat)
{

  
  double conditional_entropy = 0.0;
  /*TODO*/
  USER_ERROR("Not implemented yet");
  return conditional_entropy;
}

/*Conditonal entropy with binary feature
 */
double conditional_entropy_data_b(SSVINFO* ssvinfo_p,
				  uchar* member_data_mask,
				  size_t num_member_data,
				  size_t i_feat)
{
  double conditional_entropy = 0.0;
  /*TODO*/
  USER_ERROR("Not implemented yet");
  return conditional_entropy;
}


int find_max_gain_feature(SSVINFO* ssvinfo_p,
			  uchar* member_data_mask,
			  size_t num_member_data)
{
  double entropy_orig  = entropy_data(ssvinfo_p, member_data_mask, num_member_data);
  int    max_gain_feat = -1;
  double max_gain      = 0.0;

  //printf("entropy_orig:%lf\n", entropy_orig);
  
  //The first feature is the feature to predict. Skip this one
  for(size_t i_feat = 1; i_feat < ssvinfo_p->num_feats; ++i_feat)
    {
      char   feat_type     = ssvinfo_p->feat_types[i_feat];
      char*  feat_name     = ssvinfo_p->feat_names[i_feat];
      double entropy_condi = 0.0;
      double gain          = 0.0;

      //printf("%s %c\n", feat_name, feat_type);
      
      switch(feat_type)
	{
	case 'b':
	  entropy_condi = conditional_entropy_data_b(ssvinfo_p,
						     member_data_mask,
						     num_member_data,
						     i_feat);
	  break;
	case 'd':
	  entropy_condi = conditional_entropy_data_d(ssvinfo_p,
						     member_data_mask,
						     num_member_data,
						     i_feat);
	  break;
	case 'c':
	  entropy_condi = conditional_entropy_data_c(ssvinfo_p,
						     member_data_mask,
						     num_member_data,
						     i_feat);
	  break;
	default:
	  USER_ERROR0("Type %c not found", feat_type);
	}

      
      gain = entropy_orig - entropy_condi;
      
      //printf("gain condi: %lf\n", gain);
      //printf("entropy condi: %lf\n", entropy_condi);
      
      if(gain > max_gain)
	{
	  //printf("%lf > % max_gain %lf\n", gain, max_gain);
	  max_gain      = gain;
	  max_gain_feat = i_feat;
	}
    }

  /*?? All examples are classified very well already
   * (All positive or all negative). The gain will always be 0.
   */
  if (max_gain <= 0.0)
    {
      max_gain_feat = -1;
    }

  return max_gain_feat;
}
  

/*
int main()
{
  SSVINFO ssvinfo;
  read_ssv ("data/sanity_check.ssv",  &ssvinfo);
  size_t num_pos = 0, num_neg = 0;

  uchar* member_data_mask = NULL;
  ALLOC_BITARRAY(member_data_mask, 14);

  SET_BITARRAY_RANGE_ZERO(member_data_mask, 0, 14);

  for(size_t i_byte = 0; i_byte < 2; ++i_byte)
    {
      //printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(member_data_mask[i_byte]));
    }
  
  //printf("%lf\n", entropy_data(&ssvinfo, member_data_mask, 14));

  SET_BITARRAY_RANGE_ONE(member_data_mask, 0, 14);

  for(size_t i_byte = 0; i_byte < 2; ++i_byte)
    {
      //printf(BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(member_data_mask[i_byte]));
    }

  //printf("%lf\n", entropy_data(&ssvinfo, member_data_mask, 14));
  
  double conditional_entropy = conditional_entropy_data_d(&ssvinfo, NULL, 14, 5);
  //printf("conditional entropy %lf\n", conditional_entropy);

  int feat = max_gain_feature(&ssvinfo, member_data_mask, 14);
  printf("%d\n", feat);
}
*/
