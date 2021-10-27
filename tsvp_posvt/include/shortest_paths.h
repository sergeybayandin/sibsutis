#ifndef __SHORTEST_PATHS_H__
#define __SHORTEST_PATHS_H__

#include "mvector.h"
#include "adjacency_list.h"
#include <stddef.h>

void shortest_paths_dijkstra(int*, const adjacency_list_t*, size_t);
void shortest_paths_ford_bellman(int*, const adjacency_list_t*, size_t);

#endif // __SHORTEST_PATHS_H__
