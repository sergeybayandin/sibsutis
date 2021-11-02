#include "../include/shortest_paths.h"
#include "../include/mv_algorithm.h"
#include "../include/binary_heap.h"
#include <limits.h>
#include <string.h>
#include <stdlib.h>

size_t* pi;
const int* pdist;

int __less(const size_t* ai, const size_t* bi)
{
	const size_t v0 = *ai, v1 = *bi;
	const int res = pdist[v0] - pdist[v1];
	if (res < 0) {
		size_t t = pi[v0];
		pi[v0] = pi[v1];
		pi[v1] = t;
	}
	return res;
}

void __decrease_key(void*, const void*) {}

void shortest_paths_dijkstra(int* pd, int* pp, const adjacency_list_t* pg, size_t v)
{
	int w;
	const struct wvertex* pwv_v;
	struct binary_heap bh, *pbh = &bh;
	size_t i, m, n = MV_NUM(pg), *ps;
	struct mvector* pmv_heap = &bh.mv_heap, *pmv_g = MV_MEMORY(pg), *pmv_v;
	if ((pi = malloc(n * sizeof *pi)) == NULL)
		return;
	if ((bh = bh_malloc(n, sizeof(size_t), 
		(int (*)(const void*, const void*))__less, 
			__decrease_key, NULL), MV_MEMORY(pmv_heap)) == NULL) {
		free(pi);
		return;
	}
	memset(pd, CHAR_MAX, n * sizeof *pd); pd[v] = 0; 
	for (i = 0, pdist = pd, ps = &i; i < n; ++i) {		
		pi[i] = i;
		bh_insert(pbh, ps);
	}	
	for (ps = &v; MV_NUM(pmv_heap) != 0; ) {
		bh_erase(pbh, 0, ps);
		pmv_v = pmv_g + v; pwv_v = MV_MEMORY(pmv_v);
		for (i = 0, m = MV_NUM(pmv_v); i < m; ++i) {
			w = pwv_v[i].w + pd[v]; n = pwv_v[i].v;
			if (pd[n] <= w)
				continue;
			pd[n] = w;
			pp[n] = v;
			bh_decrease_key(pbh, pi[n], NULL);
		}
	}
	bh_free(pbh); free(pi);
}

void shortest_paths_ford_bellman(int* pd, int* pp, const adjacency_list_t* pg, size_t v)
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
				w = pd[i] + pwv[j].w; k = pwv[j].v;
				if (pd[k] <= w)
					continue;
				pd[k] = w;
				pp[k] = i;
				relax = 1;
			}
		}
	}
}
