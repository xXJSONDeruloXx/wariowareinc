#include "global.h"
#include "scenes.h"

extern void func_08007000(u32, s32, s32);

void func_08016B14(void) {
    u8 *base = (u8 *)gCurrentSceneData;

    func_08007000((u32)(base + 0x3C), *(s16 *)(base + 0x48), 0);
}
