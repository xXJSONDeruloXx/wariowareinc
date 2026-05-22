#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_080021C8(u32);

u32 func_08002584(u32 arg0) {
    u8 val;
    u32 result;
    func_080021C8(arg0);
    val = *(u8 *)arg0;
    result = 1;
    result &= ~val;
    return result;
}
#endif
