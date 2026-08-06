#include "global.h"

void func_08022010(s32 arg0) {
    u8 *ptr = *(u8 **)((u8 *)gCurrentSceneVariable + 0x10);
    u32 count = 0;

    do {
        *(u32 *)(ptr + 0x14) = arg0;
        count++;
        ptr += 0x20;
    } while (count <= 6);
}
