#include "../include/dsu.h"
#include <stddef.h>
#include <string.h>

struct dsu dsu_malloc(size_t capacity)
{
	struct dsu dsu, *pdsu = &dsu;
	memset(pdsu, 0x00, sizeof dsu);
	struct mvector* pmv_dsu = &pdsu->mv_dsu, *pmv_rank = &pdsu->mv_rank;
	MV_SIZE(pmv_dsu) = MV_SIZE(pmv_rank) = sizeof(size_t);
	mv_reserve(pmv_dsu, capacity);
	if (MV_MEMORY(pmv_dsu) == NULL)
		return dsu;
	mv_reserve(pmv_rank, capacity);
	if (MV_MEMORY(pmv_rank) == NULL)
		mv_free(pmv_dsu);
	return dsu;
}

void dsu_make_set(struct dsu* pdsu, size_t i)
{
	struct mvector* pmv_dsu = &pdsu->mv_dsu;
	if (MV_NUM(pmv_dsu) != i)
		return;
	size_t* pi = &i;
	mv_push_back(pmv_dsu, pi);
	i = 0;
	mv_push_back(&pdsu->mv_rank, pi);
}

size_t dsu_find_set(struct dsu* pdsu, size_t x)
{
	size_t r = x, *psdsu = mv_at(&pdsu->mv_dsu, 0), y = psdsu[x];
	while (r != y) {
		r = y;	
		y = psdsu[r];
	}
	y = psdsu[x];
	while (y != r) {
		psdsu[x] = r;
		x = y;
		y = psdsu[x];
	}
	return r;
}

size_t dsu_union_set(struct dsu* pdsu, size_t xset, size_t yset)
{
	struct mvector* pmv_rank = &pdsu->mv_rank;
	size_t* xrank = mv_at(pmv_rank, xset), *yrank = mv_at(pmv_rank, yset);
	if (*xrank < *yrank) {
		size_t t = xset;
		xset = yset;
		yset = t;
	}
	*(size_t*)mv_at(&pdsu->mv_dsu, yset) = xset;
	if (*xrank == *yrank)
		++(*xrank);
	return xset;
}

void dsu_free(struct dsu* pdsu)
{
	mv_free(&pdsu->mv_dsu);
	mv_free(&pdsu->mv_rank);
}
