#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"

extern u32 D_083AAD70[];

u8 func_08013628(void) {
    register u32 tableBase asm("r0");
    register u8 *state asm("r2");
    register u32 tablePtr asm("r1");
    register u32 index asm("r0");
    register u32 part asm("r2");

    tableBase = (u32)D_083AAD70;
    state = (u8 *)&D_03006518;
    tablePtr = state[0];
    tablePtr <<= 2;
    tablePtr += tableBase;
    index = state[3];
    index <<= 2;
    part = state[4];
    index += part;
    tablePtr = *(u32 *)tablePtr;
    index <<= 3;
    index += tablePtr;
    return *(u8 *)index;
}
#endif
