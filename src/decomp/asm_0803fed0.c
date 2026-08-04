#include "global.h"

s32 func_0803FED0(void) {
    u16 *ptr = (u16 *)0x086F277C;

    asm volatile("" : "+r"(ptr));
    return (s16)(ptr[1] + 20);
}
