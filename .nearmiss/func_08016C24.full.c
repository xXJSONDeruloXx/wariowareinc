#include "global.h"
#include "scenes.h"

extern void func_0800806C(u32, u32, u32, u32, u32, u32);

void func_08016C24(void) {
    func_0800806C(2, 0x4000, 0x4000, 0xF000, 0xA000, 0);
    *(u32 *)((u8 *)gCurrentSceneData + 0x54) = 0x40;
    *(u32 *)((u8 *)gCurrentSceneData + 0x58) = 0x40;
    *(u32 *)((u8 *)gCurrentSceneData + 0x5C) = 0xF0;
    *(u32 *)((u8 *)gCurrentSceneData + 0x60) = 0xA0;
}
