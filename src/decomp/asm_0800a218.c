#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern s32 func_08001B04(s32);

s32 func_0800A218(void) {
    s32 id;
    id = get_current_mem_id();
    return func_08001B04(id);
}
#endif
