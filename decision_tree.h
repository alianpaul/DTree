#ifndef DECISION_TREE_H
#define DECISION_TREE_H 1

#define MIN_LEAF_MEMBERS 1

#include "ssvinfo_p"

typedef struct dtnodes
{
  int num_examples; /*Number of examples rooted at this node*/
  int num_pos;      /*Number of positive examples*/
  int num_neg;      /*Number of negative examples*/

  int num_children;
  struct dtnodes **children;
  
} DTNODES;

/*
 */
DTNODES *create_decision_tree(ssvinfo *ssvinfo_p);

#endif
