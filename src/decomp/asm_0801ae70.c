#include "global.h"
#include "types.h"
#include "graphics.h"

struct Func0801AE70Graphics {
    u16 field0;
};

void func_0801AE70(void) {
    struct Func0801AE70Graphics *graphics;
    u32 value;
    u32 mask;

    graphics = (struct Func0801AE70Graphics *)&gGraphicsBuffer;
    value = graphics->field0;
    mask = 0xE0FF;
    mask &= value;
    graphics->field0 = mask;
}
