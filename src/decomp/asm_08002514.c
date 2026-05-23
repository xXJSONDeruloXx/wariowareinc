#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_080024D0(u32 *, u32, u32, u32);

void func_08002514(u32 *table, u32 a1, u32 a2, u32 a3) {
    register u32 r0 asm("r0");
    register u32 r4 asm("r4") = (u32)table;

    goto check;
    loop:
    r4 += 0xC;
    check:
    r0 = *(u32 *)r4;
    if (r0 != 0) goto loop;
    r0 = r4;
    func_080024D0((u32 *)r0, a1, a2, a3);
}

__attribute__((section(".text"))) const u16 _padding = 0;
#endif
