#include "../include/adjacency_list.h"
#include "../include/mvector.h"
#include "../include/shortest_paths.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
	size_t n, v;
	printf("Enter n, v: "); scanf("%zu %zu", &n, &v);
	adjacency_list_t g;
	if ((g = mv_malloc(n, sizeof(struct mvector)), MV_MEMORY(&g)) == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		return EXIT_FAILURE;
	}
	size_t i, j, a, m;
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
	int* d;
	if ((d = malloc(n * sizeof *d)) == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		return EXIT_FAILURE;
	}
	puts("");
	shortest_paths_dijkstra(d, &g, v);
	for (i = 0; i < n; ++i)
		if (i != v)
			printf("%zu --- %zu : %d\n", v, i, d[i]);
	return EXIT_SUCCESS;
}
