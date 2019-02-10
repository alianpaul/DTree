#include "decision_tree.h"
#include "bitarray.h"
#include "entropy.h"
#include "utils.h"


DTNODES* sort_decision_tree(SSVINFO* ssvinfo_p,
			    uchar* member_data_mask,
			    size_t num_member_data);

DTNODES*
create_b_dtnode(SSVINFO* ssvinfo_p,
		uchar*   member_data_mask,
		size_t   num_member_data,
		int      i_feat)
{
  USER_ERROR("Not implemented yet");
  return NULL;
}

DTNODES*
create_d_dtnode(SSVINFO* ssvinfo_p,
		uchar*   member_data_mask,
		size_t   num_member_data,
		int      i_feat)
{
  DTNODES* node = (DTNODES*) getmem(sizeof(DTNODES));
  node->num_examples = num_member_data;
  count_examples(ssvinfo_p,
		 member_data_mask,
		 num_member_data,
		 &(node->num_pos),
		 &(node->num_neg));
  node->feat         = i_feat;
  node->num_children = ssvinfo_p->num_discrete_vals[i_feat];
  node->children     = (DTNODES**)getmem(node->num_children * sizeof(DTNODES*));

  size_t num_data = ssvinfo_p->num_datas;

  uchar* feat_data_mask = NULL;
  ALLOC_BITARRAY(feat_data_mask, num_data);
  size_t num_feat_data  = 0;
  
  for(int feat_val = 0; feat_val < ssvinfo_p->num_discrete_vals[i_feat]; ++feat_val)
    {
      //For each val of the feature. Find the max info gain attribute
      //and create the node.
      num_feat_data = 0;
      SET_BITARRAY_RANGE_ZERO(feat_data_mask, 0, num_data);

      for(size_t i_data = 0; i_data < num_data; ++i_data)
	{
	  if(GET_BITARRAY(member_data_mask, i_data) == 0 
	     || read_attrib_i(ssvinfo_p->data,
			      i_data,
			      i_feat) != feat_val)
	    continue;

	  num_feat_data++;
	  SET_BITARRAY_ONE(feat_data_mask, i_data);
	}

      node->children[feat_val] = sort_decision_tree(ssvinfo_p,
						    feat_data_mask,
						    num_feat_data);
    }

  free(feat_data_mask);
  return node;
}

DTNODES*
create_c_dtnode(SSVINFO* ssvinfo_p,
		uchar*   member_data_mask,
		size_t   num_member_data,
		int      i_feat)
{
  USER_ERROR("Not implemented yet");
  return NULL;
}

DTNODES*
sort_decision_tree(SSVINFO* ssvinfo_p,
		   uchar*   member_data_mask,
		   size_t   num_member_data)
{
  DTNODES* node = NULL;
  //1. Find the feature with the max information gain
  
  if(num_member_data == 0)
    {
      //Don't need to sort to create the children.
      //Use the current node as a decision node to judge result.
      return node; //NULL
    }

  //2. Still needs to create a node for the un-predicted data.
  int max_gain_feat = find_max_gain_feature(ssvinfo_p,
					    member_data_mask,
					    num_member_data);
  printf("max_gain_feature: %d %s\n",
	 max_gain_feat,
	 ssvinfo_p->feat_names[max_gain_feat]);

  if(max_gain_feat == -1)
    {
      //Have un-predicted data. but cant find a feature. So create a leaf node.
      node = (DTNODES*)getmem(sizeof(DTNODES));
      node->num_examples = num_member_data;
      count_examples(ssvinfo_p,
		     member_data_mask,
		     num_member_data,
		     &(node->num_pos),
		     &(node->num_neg));
      node->feat         = -1;
      node->num_children = 0;
      node->children     = NULL;
    }
  else
    {
      //Found an attribute, create the node and the subtree under this node.
      char feat_type = ssvinfo_p->feat_types[max_gain_feat];
      switch(feat_type)
	{
	case 'b':
	  node = create_b_dtnode(ssvinfo_p,
				 member_data_mask,
				 num_member_data,
				 max_gain_feat);
	  break;
	case 'd':
	  node = create_d_dtnode(ssvinfo_p,
				 member_data_mask,
				 num_member_data,
				 max_gain_feat);
	  break;
	case 'c':
	  node = create_c_dtnode(ssvinfo_p,
				 member_data_mask,
				 num_member_data,
				 max_gain_feat);
	  break;
	}
    }
  
  //2. Create the node with max information gain feature
  return node;
}

DTNODES*
create_decision_tree(SSVINFO* ssvinfo_p)
{
  DTNODES* root;

  size_t num_member_data  = ssvinfo_p->num_datas; //start with all the data
  uchar* member_data_mask = NULL;
  
  ALLOC_BITARRAY(member_data_mask, num_member_data);
  SET_BITARRAY_RANGE_ONE(member_data_mask, 0, num_member_data);

  root = sort_decision_tree(ssvinfo_p, member_data_mask, num_member_data);

  free(member_data_mask);
  return root;
}

int main()
{
  SSVINFO ssvinfo;
  read_ssv("data/sanity_check.ssv", &ssvinfo);
  DTNODES* dtree = create_decision_tree(&ssvinfo);
}

