#include "global.h"
#include "graphics.h"

struct GraphicsMenuRegisters {
    u16 dispcnt;
    u8 pad0[0x3A];
    u16 unk3C;
    u16 unk3E;
    u16 unk40;
    u16 unk42;
    u16 unk44;
    u16 unk46;
};

void func_08016BF0(void) {
    struct GraphicsMenuRegisters *graphics;

    graphics = (struct GraphicsMenuRegisters *)&gGraphicsBuffer;
    graphics->dispcnt |= 0x2000;
    graphics->unk3C = 0xF0;
    graphics->unk40 = 0x1987;
    graphics->unk44 = 0x3F;
    graphics->unk46 = 0;
}
