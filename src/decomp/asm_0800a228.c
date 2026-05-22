#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *func_08006184(u32, u32);

void *func_0800A228(u32 arg0) {
    return func_08006184((u16)get_current_mem_id(), arg0);
}
#endif
