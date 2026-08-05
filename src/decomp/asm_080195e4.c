#include "global.h"
#include "types.h"
#include "graphics.h"

void func_080195E4(void) {
    u8 *p;
    u8 *graphics;
    register u32 zero asm("r1");
    p = (u8 *)gCurrentSceneVariable;
    p += 0x66;
    zero = 0;
    *(u16 *)p = zero;
    graphics = (u8 *)&gGraphicsBuffer;
    graphics += 0x4C;
    *(u16 *)graphics = zero;
}
