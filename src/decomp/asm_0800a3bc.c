#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

void func_0800A3BC(void) {
    u8 *ptr;
    u8 val;
    register u32 m asm("r0");
    ptr = (u8 *)gCurrentSceneData;
    val = ptr[7];
    m = 3;
    m = -m;
    m = val & m;
    ptr[7] = m;
}
#endif
