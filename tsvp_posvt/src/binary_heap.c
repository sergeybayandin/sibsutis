#include "../include/binary_heap.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct binary_heap bh_malloc(size_t capacity, size_t size, 
	int (*comp)(const void *, const void *), 
		void (*swap)(void *, void *),
			void (*decrease_key)(void *, const void *),
				void (*increase_key)(void *, const void *))
{
	struct binary_heap bh, *pbh = &bh;
	memset(pbh, 0x00, sizeof bh);
	struct mvector* pmv_heap = &pbh->mv_heap;
	if (capacity == 0 || size == 0 || (MV_MEMORY(pmv_heap) = malloc(size * capacity)) == NULL)
		return bh;
	MV_CAPACITY(pmv_heap) = capacity;
	MV_SIZE(pmv_heap) = size;
	BH_COMP(pbh) = comp;
	BH_SWAP(pbh) = swap;
	BH_DECREASE_KEY(pbh) = decrease_key;
	BH_INCREASE_KEY(pbh) = increase_key;
	return bh;
}

size_t __sift_up(char *pbyte, size_t size, 
		size_t i, int (*comp)(const void *, const void *), void (*swap)(void *, void *))
{
	size_t p = (i - 1) / 2;
	char *ci = pbyte + i * size, *cp = pbyte + p * size;
	while (i > 0 && comp(ci, cp) < 0) {
		swap(ci, cp);
		i = p; p = (i - 1) / 2;
		ci = cp;
		cp = pbyte + p * size;
	}
	return i;
}

size_t bh_insert(struct binary_heap *pbh, const void *key)
{
	struct mvector *pmv_heap = &pbh->mv_heap;
	mv_push_back(pmv_heap, key);
	return __sift_up(MV_MEMORY(pmv_heap), MV_SIZE(pmv_heap), 
		MV_NUM(pmv_heap) - 1, BH_COMP(pbh), BH_SWAP(pbh)); 
}

size_t __sift_down(char *pbyte, size_t num, size_t size, size_t i, 
	int (*comp)(const void *, const void *), void (*swap)(void *, void *))
{
	size_t l, r, p;
	for (l = 2 * i + 1, r = l + 1, p = i; 
			l < num; i = p, l = 2 * i + 1, r = l + 1) {
		if (comp(pbyte + l * size, pbyte + p * size) < 0)
			p = l;
		if (r < num && comp(pbyte + r * size, pbyte + p * size) < 0)
			p = r;
		if (p != i)
			swap(pbyte + p * size, pbyte + i * size);
		else
			break;
	}
	return i;
}

size_t bh_erase(struct binary_heap *pbh, size_t i, void *key)
{
	struct mvector *pmv_heap = &pbh->mv_heap;
	const size_t size = MV_SIZE(pmv_heap);
	if (key != NULL)
		memcpy(key, mv_at(pmv_heap, i), size);
	mv_ferase(pmv_heap, i);
	return __sift_down(MV_MEMORY(pmv_heap), MV_NUM(pmv_heap), size, 
		i, BH_COMP(pbh), BH_SWAP(pbh));
}

size_t bh_decrease_key(struct binary_heap *pbh, size_t i, const void *pdelta)
{
	struct mvector *pmv_heap = &pbh->mv_heap;
	BH_DECREASE_KEY(pbh)(mv_at(pmv_heap, i), pdelta);
	return __sift_up(MV_MEMORY(pmv_heap), MV_SIZE(pmv_heap), 
		i, BH_COMP(pbh), BH_SWAP(pbh));
}

size_t bh_increase_key(struct binary_heap *pbh, size_t i, const void *pdelta)
{
	struct mvector *pmv_heap = &pbh->mv_heap;
	BH_INCREASE_KEY(pbh)(mv_at(pmv_heap, i), pdelta);
	return __sift_down(MV_MEMORY(pmv_heap), MV_NUM(pmv_heap), MV_SIZE(pmv_heap), 
		i, BH_COMP(pbh), BH_SWAP(pbh));
}

void bh_free(struct binary_heap *pbh)
{
	mv_free(&pbh->mv_heap);
	memset(pbh, 0x00, sizeof *pbh);
}
