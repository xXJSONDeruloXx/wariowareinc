#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *func_08002124(void *, u32, u32);
extern void func_080021C8(void *);

void load_gfx_table(u32 arg0) {
    u8 stack[0x5C];
    register u32 a0 asm("r1") = arg0;
    register u32 size asm("r2");
    register u8 *sp asm("r0");
    register u8 *r4 asm("r4");
    register u32 r5 asm("r5");
    register u8 val asm("r1");
    register u32 bit asm("r0");

    size = 0x80;
    size <<= 10;
    sp = stack;
    func_08002124(sp, a0, size);
    sp = stack;
    val = *sp;
    bit = 1;
    bit &= val;
    if (bit != 0) {
        r4 = stack;
        r5 = 1;
        do {
            sp = stack;
            func_080021C8(sp);
            val = *r4;
            bit = r5;
            bit &= val;
        } while (bit != 0);
    }
}
#endif
