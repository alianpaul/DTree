#ifndef ENTROPY_H
#define ENTROPY_H 1

#include "utils.h"
#include "ssv.h"
#include "bitarray.h"

double entropy_b(size_t num_pos, size_t num_neg);


/* train_mask to indicate which examples are used 
 * to train the decision tree.
 * 
 * return num_pos, num_neg
 */
void   count_examples(SSVINFO* ssvinfo_p,
		      uchar*   member_data_mask,
		      size_t   num_member_data,
		      size_t*  num_pos, size_t* num_neg);

#endif
