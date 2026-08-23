#include "global.h"
#include "graphics.h"

struct Func080186ACGraphics {
    u16 field0;
    u8 padding3C[0x3A];
    u16 field3C;
    u8 padding40[2];
    u16 field40;
    u8 padding44[2];
    u16 field44;
    u16 field46;
};

void func_080186AC(void) {
    struct Func080186ACGraphics *graphics;
    u32 value;
    u32 mask;
    u32 zero;

    graphics = (struct Func080186ACGraphics *)&gGraphicsBuffer;
    value = graphics->field0;
    mask = 0xDFFF;
    mask &= value;
    zero = 0;
    graphics->field0 = mask;
    graphics->field3C = zero;
    graphics->field40 = zero;
    graphics->field44 = zero;
    graphics->field46 = zero;
}
