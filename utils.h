#ifndef DECISION_TREE_UTILS_H
#define DECISION_TREE_UTILS_H 1

#include <stdlib.h>
#include <stdio.h>

/* LOG MARCOS */
#define USER_ERROR(format)                      \
{                                               \
fprintf(stderr, "%s: %d: %s(): " format "\n",   \
__FILE__, __LINE__, __FUNCTION__);		\
exit(1);                                        \
}

#define USER_ERROR0(format, arg_0)			\
{                                                       \
fprintf(stderr, "%s: %d: %s(): " format "\n",           \
	__FILE__, __LINE__, __FUNCTION__,               \
        arg_0);                                         \
exit(1);                                                \
}

#define USER_ERROR1(format, arg_0, arg_1)               \
{                                                       \
fprintf(stderr, "%s: %d: %s(): " format "\n",           \
__FILE__, __LINE__, __FUNCTION__,			\
        arg_0, arg_1);					\
exit(1);                                                \
}

#define USER_ERROR2(format, arg_0, arg_1, arg_2)        \
{                                                       \
fprintf(stderr, "%s: %d: %s(): " format "\n",           \
	__FILE__, __LINE__, __FUNCTION__,               \
        arg_0, arg_1, arg_2);                           \
exit(1);                                                \
} 

/* Functions */
void* getmem(size_t bytes);


#endif
