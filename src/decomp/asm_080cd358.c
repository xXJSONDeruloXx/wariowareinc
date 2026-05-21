#include "global.h"
#include "types.h"
#include "scenes.h"

void func_080CD358(void *arg0) {
    *(u32*)((u8*)arg0 + 0x24) = *(u16*)((u8*)gCurrentSceneData + 0x16) << 1;
}
