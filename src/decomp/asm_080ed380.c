#include "global.h"

void func_080ED380(void) {
    u8 *base = (u8 *)gCurrentSceneVariable;
    u32 value = *(u32 *)0x08124E38;

    *(u32 *)(base + 0x40) = value;
    *(u32 *)(base + 0x60) = value;
}
