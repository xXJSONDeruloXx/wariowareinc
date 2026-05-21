#include "global.h"
#include "types.h"

void func_080719B8(u32);

void func_08073660(void) {
    u8 *p;
    func_080719B8(0);
    p = (u8 *)gCurrentSceneVariable;
    *(u32 *)(p + 0x80C) = 0;
}
