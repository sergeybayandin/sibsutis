#ifndef __MVECTOR_H__
#define __MVECTOR_H__

#include <stddef.h>

struct mvector
{
	void* mv;
	size_t num, size, capacity;
};

struct mvector mv_malloc(size_t, size_t);
void 					 mv_reserve(struct mvector*, size_t);
void					 mv_push_back(struct mvector*, const void*);
void 					 mv_swap(struct mvector*, struct mvector*);
void*					 mv_at(const struct mvector*, size_t);
void					 mv_erase(struct mvector*, size_t);
void					 mv_ferase(struct mvector*, size_t);
void 					 mv_free(struct mvector*);
void 					 mv_pop_back(struct mvector*);

#define MV_CAPACITY(pmv) (pmv)->capacity
#define MV_SIZE(pmv) (pmv)->size
#define MV_NUM(pmv) (pmv)->num
#define MV_MEMORY(pmv) (pmv)->mv

#endif // __MVECTOR_H__
