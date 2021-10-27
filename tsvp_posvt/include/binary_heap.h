#ifndef __BINARY_HEAP_H__
#define __BINARY_HEAP_H__

#include "mvector.h"
#include <stddef.h>

struct binary_heap
{
	struct mvector mv_heap;
	int  (*comp)(const void*, const void*);
	void (*decrease_key)(void*, const void*);
	void (*increase_key)(void*, const void*);
};

struct binary_heap bh_malloc(size_t, size_t, 
	int (*)(const void*, const void*), 
		void (*)(void*, const void*),
			void (*)(void*, const void*));
void 							 bh_free(struct binary_heap*);
size_t 						 bh_insert(struct binary_heap*, const void*);
size_t 						 bh_erase(struct binary_heap*, size_t i, void*);
size_t 						 bh_decrease_key(struct binary_heap*, size_t, const void*);
size_t 						 bh_increase_key(struct binary_heap*, size_t, const void*);

#define BH_COMP(pbh) (pbh)->comp
#define BH_DECREASE_KEY(pbh) (pbh)->decrease_key
#define BH_INCREASE_KEY(pbh) (pbh)->increase_key

#endif // __BINARY_HEAP_H__
