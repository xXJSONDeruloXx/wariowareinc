#include "scenes.h"

void func_0809E804(s32 arg0) {
    u8 *base = (u8 *)gCurrentSceneVariable;
    u8 *data = (u8 *)gCurrentSceneData;
    u32 value = *(u16 *)(data + 0x16);

    value >>= 5;
    value *= arg0;
    base += 0xCA;
    *(u16 *)base = value;
}
