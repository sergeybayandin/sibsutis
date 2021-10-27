#include "../include/spanning_tree.h"
#include "../include/dsu.h"
#include "../include/mv_algorithm.h"
#include <stdlib.h>

int __less(const struct wedge* a, const struct wedge* b)
{
	return a->weight - b->weight;
}

int __greater(const struct wedge* a, const struct wedge* b)
{
	return b->weight - a->weight;
}

void find_spanning_tree(edge_list_t* gdest, edge_list_t* gsrc, size_t n, int sort)
{
	struct wedge* pwedge = MV_MEMORY(gsrc);
	struct dsu dsu, *pdsu = &dsu;
	size_t m = MV_NUM(gsrc), i, xset, yset;
	if ((dsu = dsu_malloc(n), MV_MEMORY(&dsu.mv_dsu)) == NULL)
		return;
	qsort(pwedge, m, MV_SIZE(gsrc), 
		(int (*)(const void*, const void*))(sort == 0 ? __less : __greater));
	for (i = 0; i < n; ++i)
		dsu_make_set(pdsu, i);
	for (i = 0; n > 1; ++i) {
		xset = dsu_find_set(pdsu, pwedge[i].from);
		yset = dsu_find_set(pdsu, pwedge[i].to);
		if (xset != yset) {
			dsu_union_set(pdsu, xset, yset);
			mv_push_back(gdest, pwedge + i);
			--n;
		}
	}
	dsu_free(pdsu);
}
