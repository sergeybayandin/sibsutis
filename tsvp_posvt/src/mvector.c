#include "../include/mvector.h"
#include <string.h>
#include <stdlib.h>

struct mvector mv_malloc(size_t num, size_t size)
{
	struct mvector mv = { NULL, 0, 0, 0 }, *pmv = &mv;
	if (num == 0 || size == 0 || (MV_MEMORY(pmv) = malloc(num * size)) == NULL)
		return mv;
	MV_NUM(pmv) = num;
	MV_SIZE(pmv) = size;
	MV_CAPACITY(pmv) = num;
	return mv;
}

void mv_reserve(struct mvector* pmv, size_t new_capacity)
{
	const size_t old_capacity = MV_CAPACITY(pmv);
	if (new_capacity <= old_capacity)
		return;
	if ((MV_MEMORY(pmv) = realloc(MV_MEMORY(pmv), new_capacity * MV_SIZE(pmv))) == NULL)
		memset(pmv, 0x00, sizeof *pmv);
	else
		MV_CAPACITY(pmv) = new_capacity;
}

void mv_push_back(struct mvector* pmv, const void* mem)
{
	const size_t capacity = MV_CAPACITY(pmv), num = MV_NUM(pmv), size = MV_SIZE(pmv);
	if (capacity <= num) {
		mv_reserve(pmv, capacity == 0 ? 1 : 2 * capacity);
		if (MV_MEMORY(pmv) == NULL)
			return;
	}
	memcpy((char*)MV_MEMORY(pmv) + num * size, mem, size);
	++MV_NUM(pmv);
}

void mv_swap(struct mvector* pmv_lhs, struct mvector* pmv_rhs)
{
	struct mvector mv_temp, *pmv_temp = &mv_temp; 
	memcpy(pmv_temp, pmv_lhs, sizeof mv_temp);
	memcpy(pmv_lhs, pmv_rhs, sizeof mv_temp);
	memcpy(pmv_rhs, pmv_temp, sizeof mv_temp);
}

void* mv_at(const struct mvector* pmv, size_t i)
{
	return (char*)MV_MEMORY(pmv) + i * MV_SIZE(pmv);
}

void mv_erase(struct mvector* pmv, size_t i)
{
	const size_t size = MV_SIZE(pmv), offset = i * size, 
		nbytes = (MV_NUM(pmv) - i - 1) * size;
	char* pbyte = (char*)MV_MEMORY(pmv) + offset;
	memcpy(pbyte, pbyte + size, nbytes);
	--MV_NUM(pmv);
}

void mv_ferase(struct mvector* pmv, size_t i)
{	
	const size_t size = MV_SIZE(pmv);
	char* pbyte = MV_MEMORY(pmv);
	memcpy(pbyte + i * size, pbyte + (MV_NUM(pmv) - 1) * size, size);
	--MV_NUM(pmv);
}

void mv_pop_back(struct mvector* pmv)
{
	--MV_NUM(pmv);
}

void mv_free(struct mvector* pmv)
{
	free(MV_MEMORY(pmv));
	memset(pmv, 0x00, sizeof *pmv);
}
