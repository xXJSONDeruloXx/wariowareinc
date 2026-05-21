#include "global.h"
#include "types.h"

void func_080C9948(u32, u32);

void func_080C992C(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_080C9948((u32)(p + 0xF4), (u32)(p + (0x8C << 1)));
}
