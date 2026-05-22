#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

void func_08013114(void) {
    u8 *ptr;
    u8 val;
    register u32 m asm("r0");
    ptr = (u8 *)gCurrentSceneData;
    ptr = ptr + 0xDD;
    val = ptr[0];
    m = 9;
    m = -m;
    m = val & m;
    ptr[0] = m;
}
#endif
