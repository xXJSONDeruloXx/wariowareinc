#include "global.h"
#include "types.h"

void func_08004B78(u32, u32);

void func_080B0608(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_08004B78(*(u32 *)(p + (0x9D << 2)), *(u32 *)(p + (0x9D << 2) + 4));
}
