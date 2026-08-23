#include "global.h"
#include "types.h"
#include "graphics.h"

struct Func0805CB5CGraphics {
    u8 pad0[0x54];
    u16 field54;
};

void func_0805CB5C(void) {
    struct Func0805CB5CGraphics *graphics = (struct Func0805CB5CGraphics *)&gGraphicsBuffer;
    u16 *field = &graphics->field54;
    u32 value;
    value = 0x3FF;
    *field = value;
}
