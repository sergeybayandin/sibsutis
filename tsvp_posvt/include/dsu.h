#ifndef __DSU_H__
#define __DSU_H__

#include "../include/mvector.h"

struct dsu
{
	struct mvector mv_dsu, mv_rank;
};

struct dsu dsu_malloc(size_t);
void 	 dsu_make_set(struct dsu*, size_t);
size_t dsu_find_set(struct dsu*, size_t);
void 	 dsu_free(struct dsu*);
size_t dsu_union_set(struct dsu*, size_t, size_t);

#endif // __DSU_H__
