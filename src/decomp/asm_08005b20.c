#include "global.h"

extern void func_08005B70();

void func_08005B20(void *arg0, u16 arg1) {
    u8 *base = (u8 *)arg0;
    u16 value0 = *(u16 *)(base + 0x4);
    u16 value1 = *(u16 *)(base + 0x6);

    func_08005B70(arg0, 0, 0, value0, value1, arg1);
}
