#ifndef __SHORTEST_PATHS_H__
#define __SHORTEST_PATHS_H__

#include "mvector.h"
#include "adjacency_list.h"
#include <stddef.h>

void shortest_paths_dijkstra(int *, size_t *, const adjacency_list_t *, int);
void shortest_paths_ford_bellman(int *, size_t *, const adjacency_list_t *, int);

#endif // __SHORTEST_PATHS_H__
