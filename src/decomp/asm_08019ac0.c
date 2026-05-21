#include "global.h"
#include "types.h"

void func_0800C69C(u32, u32);

void func_08019AC0(u32 arg0) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_0800C69C(*(u32 *)(p + (0x90 << 1)), arg0);
}
