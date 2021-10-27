#ifndef __EDGE_LIST_H__
#define __EDGE_LIST_H__

#include "mvector.h"
#include <stddef.h>

struct wedge
{
	int weight;
	size_t from, to;
};

typedef struct mvector edge_list_t;

#endif // __EDGE_LIST_H__
