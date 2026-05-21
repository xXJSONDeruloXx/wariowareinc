#include "global.h"
#include "types.h"

void func_0808A56C(u32 arg0) {
    u8 *p = (u8*)gCurrentSceneVariable;
    *(u16*)(p + 0x3C) = 0;
    p[0x3B] = 1;
    *(u16*)((u8*)gCurrentSceneVariable + 0x3E) = arg0;
}
