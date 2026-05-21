#include "global.h"
#include "types.h"

void func_0800D38C(u32);

void func_080CF804(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_0800D38C(*(u32 *)(p + (0xCA << 2)));
}
