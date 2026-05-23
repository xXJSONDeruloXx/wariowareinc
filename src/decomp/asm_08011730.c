#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern struct GraphicsBuffer gGraphicsBuffer;
extern void func_0800A000(u32);

void func_08011730(u32 arg0) {
    register u32 val asm("r1");
    register u8 *ptr asm("r0");
    register u32 param asm("r0");

    val = arg0;
    if (val != 0) {
        ptr = (u8 *)&gGraphicsBuffer;
        ptr += 0x50;
        val = 4;
        *(u16 *)ptr = val;
        param = 0xB3;
        func_0800A000(param);
    } else {
        ptr = (u8 *)&gGraphicsBuffer;
        ptr += 0x50;
        *(u16 *)ptr = val;
        param = 0x80;
        param <<= 1;
        func_0800A000(param);
    }
}
#endif
