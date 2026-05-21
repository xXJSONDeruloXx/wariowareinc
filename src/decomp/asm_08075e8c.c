#include "global.h"
#include "types.h"

void func_080DF28C(u32, u16);

void func_08075E8C(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_080DF28C(*(u32 *)(p + 0x7C), *(u16 *)(p + 0x28));
}
