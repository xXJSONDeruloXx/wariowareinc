#if __INCLUDE_LEVEL__ > 0
#include "global.h"

s32 func_08002468(u8 *ptr) {
    s32 result;
    asm volatile(
        ".syntax unified\n"
        "ldrb %0, [%1]\n"
        "lsls %0, %0, #31\n"
        "lsrs %0, %0, #31\n"
        ".syntax divided\n"
        : "=r"(result) : "r"(ptr)
    );
    return result;
}
#endif
