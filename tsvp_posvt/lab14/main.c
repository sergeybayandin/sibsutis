#include "../include/adjacency_list.h"
#include "../include/mvector.h"
#include "../include/shortest_paths.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main()
{
	size_t n, v;
	printf("Enter n, v: "); scanf("%zu %zu", &n, &v);
	adjacency_list_t g;
	if ((g = mv_malloc(n, sizeof(struct mvector)), MV_MEMORY(&g)) == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		return EXIT_FAILURE;
	}
	size_t i, j, a, b, m;
	struct wvertex* pwv;
	struct mvector* pmv = MV_MEMORY(&g);
	for (i = 0; i < n; ++i) {
		scanf("%zu %zu", &a, &m);
		if (m != 0 && (pmv[a] = mv_malloc(m, sizeof(struct wvertex)), 
				MV_MEMORY(pmv + a)) == NULL) {
			fprintf(stderr, "Cannot allocate memory\n");
			return EXIT_FAILURE;
		}
		pwv = MV_MEMORY(pmv + a);
		for (j = 0; j < m; ++j)
			scanf("%zu %d", &pwv[j].v, &pwv[j].w);
	}
	puts("");
	size_t *pp;
	int* pd, *pst;
	if ((pd = malloc(n * sizeof *pd)) == NULL ||
				(pp = malloc(n * sizeof *pp)) == NULL ||
					(pst = malloc(n * sizeof *pst)) == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		return EXIT_FAILURE;
	}
	for (i = 0; i < n; ++i)
		pd[i] = INT_MAX;
	pd[v] = 0;
	for (i = 0; i < n; ++i)
		pp[i] = n;
	pp[v] = v;
	if (MV_NUM(pmv + v) > 0)
		shortest_paths_ford_bellman(pd, pp, &g, INT_MAX);	
	puts("");
	for (i = 0, m = 0; i < n; ++i) {
		if (i == v)
			continue;
		pst[m++] = i;
		if (pp[i] != n) {
			for (j = i; pp[j] != j; j = pp[j])
				pst[m++] = pp[j];
		} else {
			pst[m++] = v;
		}
		for (--m; m > 0; --m)
			printf("%d -- ", pst[m]);
		if (pp[i] != n)
			printf("%d : %d\n", pst[m], pd[i]);
		else
			printf("%d : %s\n", pst[m], "doesnt exist");
	}
	return EXIT_SUCCESS;
}
