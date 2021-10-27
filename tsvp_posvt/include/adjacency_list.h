#ifndef __ADJACENCY_LIST_H__
#define __ADJACENCY_LIST_H__

#include "mvector.h"
#include <stddef.h>

struct wvertex
{
	size_t v;
	int w;
};

typedef struct mvector adjacency_list_t;

#endif // __ADJACENCY_LIST_H__
