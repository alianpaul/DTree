#ifndef SSV_H
#define SSV_H

#include "utils.h"

typedef struct ssvinfo {
  void**  data;
  char*   feat_types;
  char**  feat_names;
  char*** discrete_vals;
  int*    num_discrete_vals;
  size_t  num_feats;
  size_t  num_datas;
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
void read_ssv_to_data(char* filename, SSVINFO* ssvinfo);

#endif
