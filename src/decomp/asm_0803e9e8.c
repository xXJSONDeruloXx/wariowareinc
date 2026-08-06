#include "global.h"

extern void func_080DF28C(u32, u16);

void func_0803E9E8(void) {
    u8 *base = (u8 *)gCurrentSceneVariable;

    func_080DF28C(*(u32 *)(base + (0x86 << 1)), *(u16 *)(base + 0x60));
}
