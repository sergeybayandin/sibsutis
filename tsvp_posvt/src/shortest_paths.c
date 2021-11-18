#include "../include/shortest_paths.h"
#include "../include/mv_algorithm.h"
#include "../include/binary_heap.h"
#include <stdlib.h>
#include <stdio.h>

int *pdist;
size_t *ppos;

int __less(const size_t *a, const size_t *b)
{
	return pdist[*a] - pdist[*b];
}

void __swap(size_t *a, size_t *b)
{
	size_t t = *a;
	*a = *b;
	*b = t;
	t = ppos[*a];
	ppos[*a] = ppos[*b];
	ppos[*b] = t;
}

void __decrease_key(size_t *, const int *) {}

#define __CAST_CMP_FUNC(pfunc) \
	(int (*)(const void *, const void *))(pfunc)

#define __CAST_SWP_FUNC(pfunc) \
	(void (*)(void *, void *))(pfunc)

#define __CAST_DK_FUNC(pfunc) \
	(void (*)(void *, const void *))(pfunc)

void shortest_paths_dijkstra(int *pd, size_t *pp, const adjacency_list_t* pg, int max_dist)
{
	size_t num = MV_NUM(pg);
	if ((ppos = malloc(num * sizeof *ppos)) == NULL)
		return;
	struct binary_heap bh, *pbh = &bh;
	const struct mvector *pmv_heap = &pbh->mv_heap;
	if ((bh = bh_malloc(num, sizeof(size_t), __CAST_CMP_FUNC(__less), __CAST_SWP_FUNC(__swap),
			__CAST_DK_FUNC(__decrease_key), NULL), MV_MEMORY(pmv_heap)) == NULL) {
		free(ppos);
		return;
	}
	size_t j, i, *ps;
	for (i = 0, ps = &i, pdist = pd; i < num; ++i) {
		ppos[i] = i;
		bh_insert(pbh, ps);
	}
	int w;
	size_t v, k;	
	const struct wvertex *pwv;
	const struct mvector *pmv, *pmv_g = MV_MEMORY(pg);
	for (ps = &v; MV_NUM(pmv_heap) != 0; ) {
		bh_erase(pbh, 0, ps);	
		if (pd[v] == max_dist)
			continue;
		pmv = pmv_g + v;
		pwv = MV_MEMORY(pmv);
		for (i = 0, num = MV_NUM(pmv); i < num; ++i) {
			w = pd[v] + pwv[i].w;
			k = pwv[i].v;
			if (pd[k] <= w)
				continue;
			pp[k] = v;
			pd[k] = w;
			bh_decrease_key(pbh, ppos[k], NULL);
		}
	}
	free(ppos);
	bh_free(pbh);
}

void shortest_paths_ford_bellman(int *pd, size_t *pp, const adjacency_list_t *pg, int max_dist)
{
	int not_relax = 1, w;
	struct wvertex* pwv;
	size_t i, j, k, n = MV_NUM(pg), m;
	struct mvector* pmv_g = MV_MEMORY(pg), *pmv_v;
	while (not_relax) {
		not_relax = 0;
		for (i = 0; i < n; ++i) {				
			if (pd[i] == max_dist)
				continue;
			pmv_v = pmv_g + i;
			pwv = MV_MEMORY(pmv_v);
			for (j = 0, m = MV_NUM(pmv_v); j < m; ++j) {
				w = pd[i] + pwv[j].w; k = pwv[j].v;
				if (pd[k] < w)
					continue;
				pd[k] = w;
				pp[k] = i;
				not_relax = 1;
			}
		}
	}
}
