#include "scenes.h"

void func_080DA0B0(void) {
    u8 *variable = (u8 *)gCurrentSceneVariable;
    u8 *scene = (u8 *)gCurrentSceneData;
    *(u32 *)(variable + 0x14) += *(u16 *)(scene + 0x16);
}
