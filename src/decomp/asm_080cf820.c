#include "global.h"
#include "types.h"

void func_0800D320(u32);

void func_080CF820(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_0800D320(*(u32 *)(p + (0xCA << 2)));
}
