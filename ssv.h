#ifndef SSV_H
#define SSV_H

#include "utils.h"

typedef struct ssvinfo {
  void**  data;
  char*   feat_types;
  char**  feat_names;
  char*** discrete_vals;     //for feat type d
  int*    num_discrete_vals; //for feat type d
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
void read_ssv (char* filename_in,  SSVINFO* ssvinfo_p);
void write_ssv(char* filename_out, SSVINFO  ssvinfo  );

#endif
