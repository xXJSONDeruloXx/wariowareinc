#include "global.h"
#include "scenes.h"

extern void func_08007000(u32, s32, s32);

void func_080241E8(void) {
    u8 *base = (u8 *)gCurrentSceneVariable;

    func_08007000((u32)(base + 0x84), *(s16 *)(base + 0x80), 0);
}
