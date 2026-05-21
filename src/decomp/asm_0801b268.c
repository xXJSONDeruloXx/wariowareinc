#include "global.h"
#include "types.h"

void func_0800C69C(u32, u32);

void func_0801B268(u32 arg0) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    func_0800C69C(*(u32 *)(p + 0xC0), arg0);
}
