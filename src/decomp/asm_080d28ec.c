#include "global.h"
#include "types.h"

void func_080DF28C(u32, u16);

void func_080D28EC(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_080DF28C(*(u32 *)(p + (0xF9 << 2)), *(u16 *)(p + (0xF9 << 2) + 0xA));
}
