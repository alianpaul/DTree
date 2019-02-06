#include "entropy.h"

#include <math.h>

double entropy_b(size_t num_pos, size_t num_neg)
{
  double prob_pos = ((double)num_pos) / (double) (num_pos + num_neg);
  double prob_neg = ((double)num_neg) / (double) (num_pos + num_neg);

  return -prob_pos * log2(prob_pos) -prob_neg * log2(prob_neg);
}

void   count_examples(SSVINFO* ssvinfo_p,
		      uchar*   train_data_mask,
		      size_t   num_train_data,
		      size_t*  num_pos, size_t* num_neg)
{
  *num_pos = 0;
  *num_neg = 0;

  for(size_t i_data = 0; i_data < ssvinfo_p->num_datas; ++i_data)
    {
      //inference on the first binary attribute
      if(train_data_mask && !GET_BITARRAY(train_data_mask, i_data))
	continue;
      
      if(read_attrib_b(ssvinfo_p->data, i_data, 0))
	++(*num_pos);
      else
	++(*num_neg);
    }
}

int main()
{
  //printf("%f\n", entropy_b(9, 5));
  SSVINFO ssvinfo;
  read_ssv ("data/sanity_check.ssv",  &ssvinfo);
  size_t num_pos = 0, num_neg = 0;
  count_examples(&ssvinfo, NULL, 0, &num_pos, &num_neg);
  printf("%zu %zu\n", num_pos, num_neg);
}

