#ifndef SSV_H
#define SSV_H

#include "utils.h"

/*
 * discrete_vals[i_feat][i_discrete] //for feat type d, type name
 * num_discrete [i_feat][i_discrete] //for feat type d, type name
 */

typedef struct ssvinfo {
  void**  data;
  char*   feat_types;
  char**  feat_names;
  size_t  num_feats;
  char*** discrete_vals;     //for feat type d
  int*    num_discrete_vals; //for feat type d
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
void write_ssv(char* filename_out, SSVINFO* ssvinfo_p);

unsigned char read_attrib_b (void **data, int example, int feature);
void write_attrib_b(void **data, int example, int feature, unsigned char val);

int read_attrib_i(void **data, int example, int feature);
void write_attrib_i(void **data, int example, int feature, int val);

#endif
