#include "../include/shortest_paths.h"
#include "../include/mv_algorithm.h"
#include "../include/binary_heap.h"
#include <limits.h>
#include <string.h>

int __less(const struct wvertex* a, const struct wvertex* b)
{
	return a->w - b->w;
}

void shortest_paths_dijkstra(int* pd, const adjacency_list_t* pg, size_t v)
{
	int w;
	size_t i, m, j, n = MV_NUM(pg);
	struct binary_heap bh, *pbh = &bh;
	struct wvertex wv = { v, 0 }, *pwv = &wv, *pwv_v, twv, *ptwv = &twv;
	struct mvector* pmv_heap = &bh.mv_heap, *pmv_g = MV_MEMORY(pg), *pmv_v;
	if ((bh = bh_malloc(n, sizeof(wv), 
		(int (*)(const void*, const void*))__less, NULL, NULL), MV_MEMORY(pmv_heap)) == NULL)
		return;
	memset(pd, CHAR_MAX, n * sizeof *pd); pd[v] = 0;
	bh_insert(pbh, pwv);
	while (MV_NUM(pmv_heap) != 0) {
		bh_erase(pbh, 0, pwv); j = pwv->v;
		if (pwv->w != pd[j])
			continue;
		pmv_v = pmv_g + j; pwv_v = MV_MEMORY(pmv_v);
		for (i = 0, m = MV_NUM(pmv_v); i < m; ++i) {
			w = pwv_v[i].w + pwv->w; j = pwv_v[i].v;
			if (pd[j] <= w)
				continue;
			pd[j] = w;
			ptwv->v = j; ptwv->w = w;
			bh_insert(pbh, ptwv);
		}
	}
	bh_free(pbh);
}

void shortest_paths_ford_bellman(int* pd, const adjacency_list_t* pg, size_t v)
{
	int relax = 1, w;
	struct wvertex* pwv;
	size_t i, j, k, n = MV_NUM(pg), m;
	struct mvector* pmv_g = MV_MEMORY(pg), *pmv_v;
	memset(pd, CHAR_MAX, n * sizeof *pd); pd[v] = 0;
	while (relax != 0) {
		relax = 0;
		for (i = 0; i < n; ++i) {				
			if (pd[i] == INT_MAX)
				continue;
			pmv_v = pmv_g + i;
			pwv = MV_MEMORY(pmv_v);
			for (j = 0, m = MV_NUM(pmv_v); j < m; ++j) {
				w = pd[i] + pwv[j].w;
				k = pwv[j].v;
				if (pd[k] > w) {
					pd[k] = w;
					relax = 1;
				}
			}
		}
	}
}
