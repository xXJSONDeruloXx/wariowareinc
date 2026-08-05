#include "global.h"
#include "types.h"
#include "scenes.h"

void func_080B36B0(void *arg0) {
    *(u32 *)((u8 *)arg0 + 0x3C) += *(u16 *)((u8 *)gCurrentSceneData + 0x16) >> 3;
}
