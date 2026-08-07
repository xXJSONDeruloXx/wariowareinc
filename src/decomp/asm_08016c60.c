#include "global.h"
#include "graphics.h"
#include "scenes.h"
#include "src/beatscript.h"

extern void func_0800806C(u32, u32, u32, u32, u32, u32);

void func_08016C60(void) {
    if (D_03006520 == 0x43) {
        gGraphicsBuffer.BG_OFS[1].x += *(u16 *)((u8 *)gCurrentSceneData + 0x50);
        func_0800806C(
            2,
            *(u32 *)((u8 *)gCurrentSceneData + 0x54) << 8,
            *(u32 *)((u8 *)gCurrentSceneData + 0x58) << 8,
            *(u32 *)((u8 *)gCurrentSceneData + 0x5C) << 8,
            *(u32 *)((u8 *)gCurrentSceneData + 0x60) << 8,
            0);
    }
}
