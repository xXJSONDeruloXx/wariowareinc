#include "global.h"
#include "types.h"
#include "scenes.h"

void func_080E1A80(void *arg0) {
    *(u32 *)((u8 *)arg0 + 0x28) += *(u16 *)((u8 *)gCurrentSceneData + 0x16) >> 3;
}
