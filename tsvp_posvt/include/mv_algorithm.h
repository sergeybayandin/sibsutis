#ifndef __MV_ALGORITHM_H__
#define __MV_ALGORITHM_H__

#include "mvector.h"

void   mv_copy(struct mvector*, const struct mvector*);
void   mv_move(struct mvector*, struct mvector*);
void 	 mv_fill(struct mvector*, const void*);

#endif // __MV_ALGORITHM_H__
