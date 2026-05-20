#include "global.h"
#include "types.h"

void func_08040A2C(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    *(u32 *)(p + 0x68) += *(u32 *)(p + 0x6C);
}
