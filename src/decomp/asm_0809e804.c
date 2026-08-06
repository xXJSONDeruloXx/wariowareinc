#include "scenes.h"

void func_0809E804(s32 arg0) {
    register u8 *base asm("r2") = (u8 *)gCurrentSceneVariable;
    register u32 value asm("r0") = arg0;
    register u32 factor asm("r1") = *(u16 *)((u8 *)gCurrentSceneData + 0x16);

    factor >>= 5;
    value *= factor;
    base += 0xCA;
    *(u16 *)base = value;
}
