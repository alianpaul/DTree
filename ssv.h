#ifndef SSV_H
#define SSV_H

#include "utils.h"

typedef struct ssvinfo {
  char*   types;
  char**  feat_names;
  char*** discrete_vals;
  int*    num_discrete_vals; 
} SSVINFO;


/*
 * Input:  
 *   filename
 * Output:
 *   return 
 *     void** data; data[feat_idx][data_idx]
 *   size_t num_feat
 *   size_t num_data
 *   SSVINFO ssvinfo
 */
void** read_ssv_file(char* filename,
		     size_t* num_feat_p,
		     size_t* num_data_p,
		     SSVINFO* ssvinfo);



#endif
