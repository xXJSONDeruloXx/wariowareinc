#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void func_0801429C(u32, u32);
extern void func_0800C7A4(s32);

void func_08014354(void) {
    u32 i;
    for (i = 0; i <= 2; i++) {
        func_0801429C(i, 0);
    }
    func_0800C7A4(0x12);
}
#endif
