#include "global.h"
#include "types.h"

void func_080C954C(u32, u32);

void func_080C9534(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_080C954C((u32)(p + 0x40), (u32)(p + 0x64));
}
