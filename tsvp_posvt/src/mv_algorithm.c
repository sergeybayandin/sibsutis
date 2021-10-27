#include "../include/mv_algorithm.h"
#include <string.h>

void mv_fill(struct mvector* pmv, const void* value)
{
	const size_t size = MV_SIZE(pmv);
	char* pfirst = MV_MEMORY(pmv), *plast = mv_at(pmv, MV_NUM(pmv));
	for (; pfirst < plast; pfirst += size)
		memcpy(pfirst, value, size);
}

void mv_copy(struct mvector* pmv_dest, const struct mvector* pmv_src)
{
  memcpy(MV_MEMORY(pmv_dest), 
		MV_MEMORY(pmv_src), MV_SIZE(pmv_src) * MV_NUM(pmv_src));
}

void mv_move(struct mvector* pmv_dest, struct mvector* pmv_src)
{
  const size_t nbytes = MV_SIZE(pmv_src) * MV_NUM(pmv_src);
  memcpy(MV_MEMORY(pmv_dest), MV_MEMORY(pmv_src), nbytes);
  memset(pmv_src, 0x00, nbytes);
}

void __swap(void* lhs, void* rhs, size_t size)
{
  char temp_buff[size]; memcpy(temp_buff, lhs, size);
  memcpy(lhs, rhs, size);
  memcpy(rhs, temp_buff, size);
}
