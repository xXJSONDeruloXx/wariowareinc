#include "global.h"
#include "types.h"
#include "graphics.h"

typedef struct {
    u16 dispcnt;
    u8 padding2[0x10];
    u16 field12;
} Func0801F1A0Graphics;

void func_0801F1A0(void) {
    Func0801F1A0Graphics *graphics;
    u32 value;
    u32 mask;

    graphics = (Func0801F1A0Graphics *)&gGraphicsBuffer;
    value = graphics->dispcnt;
    mask = 0xFEFF;
    mask &= value;
    graphics->dispcnt = mask;
    graphics->field12 = 1;
}
