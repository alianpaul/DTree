#ifndef DECISION_TREE_H
#define DECISION_TREE_H 1

#define MIN_LEAF_MEMBERS 1

#include "ssv.h"

typedef struct dtnodes
{
  size_t num_examples; /*Number of examples rooted at this node*/
  size_t num_pos;      /*Number of positive examples*/
  size_t num_neg;      /*Number of negative examples*/

  int feat;
  int num_children;
  struct dtnodes **children;
  
} DTNODES;

/*
 */
DTNODES *create_decision_tree(SSVINFO* ssvinfo_p);

#endif
