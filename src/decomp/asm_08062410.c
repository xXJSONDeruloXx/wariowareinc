#include "global.h"

extern u32 func_08008AA4(u32);

void func_08062410(void) {
    *(u32 *)((u8 *)gCurrentSceneVariable + 0xBCC) = func_08008AA4(0x11);
}
