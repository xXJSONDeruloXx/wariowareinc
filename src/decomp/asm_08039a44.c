#include "global.h"
#include "graphics.h"

struct Func08039A44Graphics {
    u16 field0;
    u8 padding4C[0x4A];
    u16 field4C;
    u16 field4E;
};

void func_08039A44(void) {
    struct Func08039A44Graphics *graphics;
    u32 value;
    u32 mask;
    u32 zero;

    graphics = (struct Func08039A44Graphics *)&gGraphicsBuffer;
    value = graphics->field0;
    mask = 0x7FFF;
    mask &= value;
    zero = 0;
    graphics->field0 = mask;
    graphics->field4C = zero;
    graphics->field4E = zero;
}
