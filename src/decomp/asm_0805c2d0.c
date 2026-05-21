#include "global.h"
#include "types.h"

void func_0805C048(s32);

void func_0805C2D0(void) {
    func_0805C048(*(s32 *)((u8 *)gCurrentSceneVariable + (0x90 << 1)) >> 9);
}
