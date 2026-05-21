#include "global.h"
#include "types.h"

void func_08001B28(s32);

void func_080421E8(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_08001B28((s32)(s8)p[0xBE]);
}
