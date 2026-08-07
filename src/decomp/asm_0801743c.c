#include "global.h"
#include "graphics.h"

extern void func_080172A8(void);
extern void func_0800C7A4(u32);
extern void func_0800BF7C(s32, u32, s16, s16, u32, u32, u32);
extern void func_0800A200(u32);
extern void func_08009EE0_stub(u32);

struct GraphicsInitRegisters {
    u16 DISPCNT;
    u8 pad[0x46];
    u16 unk48;
    u8 pad4a[2];
    u16 unk4C;
};

void func_0801743C(void) {
    struct GraphicsInitRegisters *graphics;

    func_080172A8();
    func_0800C7A4(0);
    graphics = (struct GraphicsInitRegisters *)&gGraphicsBuffer;
    graphics->DISPCNT = 0x1000;
    graphics->unk4C = 0;
    graphics->unk48 = 0;
    func_0800BF7C(0, 1, 0, 0, 2, 0x1C, 0);
    gGraphicsBuffer.unk854_1 = 1;
    func_0800A200(1);
    func_08009EE0_stub(0);
}
