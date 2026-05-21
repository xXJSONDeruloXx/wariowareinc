#include "global.h"
#include "types.h"

void func_08001B28(s32);

void func_08038F6C(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_08001B28((s32)(s8)p[0x98]);
}
