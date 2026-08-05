#include "global.h"
#include "types.h"

void func_080AAA40(u32 index, u16 value) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    u32 offset = index << 1;
    u32 base = 0x83;
    base <<= 2;
    p += base;
    p += offset;
    *(u16 *)p = value;
}
