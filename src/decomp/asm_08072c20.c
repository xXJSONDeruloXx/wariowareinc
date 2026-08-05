#include "global.h"

void func_08072C20(void *arg0, s32 arg1) {
    u8 *ptr = (u8 *)arg0;
    *(u32 *)(ptr + 0x14) = arg1;
    if (arg1 <= 0) {
        *(u32 *)(ptr + 0x14) = 4;
    }
}
