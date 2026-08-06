#include "global.h"

void func_08072DD4(void *arg0) {
    u8 *p = (u8 *)arg0;

    if (*(s32 *)(p + 0x18) == 1) {
        *(s32 *)(p + 0x18) = 0;
        *(u16 *)*(u32 **)(p + 4) = *(u16 *)(p + 0xE);
    }
}
