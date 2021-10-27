#include "../include/spanning_tree.h"
#include "../include/edge_list.h"
#include "../include/mvector.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{
	size_t m, n, i;
	printf("Enter m, n: "); scanf("%zu %zu", &m, &n);
	edge_list_t gsrc, gdest = { NULL, 0, sizeof(struct wedge), 0 };
	if ((gsrc = mv_malloc(m, sizeof(struct wedge)), MV_MEMORY(&gsrc)) == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		return EXIT_FAILURE;
	}	
	if ((mv_reserve(&gdest, m), MV_MEMORY(&gdest)) == NULL) {
		fprintf(stderr, "Cannot allocate memory\n");
		return EXIT_FAILURE;
	}
	struct wedge* pwedge = MV_MEMORY(&gsrc);
	for (i = 0; i < m; ++i)
		scanf("%zu %zu %d", &pwedge[i].from, &pwedge[i].to, &pwedge[i].weight);
	find_spanning_tree(&gdest, &gsrc, n, 0);
	pwedge = MV_MEMORY(&gdest);
	m = gdest.num;
	for (i = 0; i < m; ++i)
		printf("%zu --- %zu : %d\n", pwedge[i].from, pwedge[i].to, pwedge[i].weight);
	puts("");
	return EXIT_SUCCESS;	
}
