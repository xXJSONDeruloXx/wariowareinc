#include "global.h"
#include "types.h"

void func_08052538(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    *(u32 *)(p + 0x80) += 0x50;
}
