#include "scenes.h"

void func_0809E804(s32 arg0) {
    u8 *scene;
    u16 value;

    scene = (u8 *)gCurrentSceneVariable;
    value = *(u16 *)((u8 *)gCurrentSceneData + 0x16);
    value >>= 5;
    *(u16 *)(scene + 0xCA) = value * arg0;
}
