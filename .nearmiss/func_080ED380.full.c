#include "global.h"

extern u32 D_08124E38;

void func_080ED380(void) {
    *(u32 *)((u8 *)gCurrentSceneVariable + 0x40) = D_08124E38;
    *(u32 *)((u8 *)gCurrentSceneVariable + 0x60) = D_08124E38;
}
