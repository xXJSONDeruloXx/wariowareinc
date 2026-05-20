#include "global.h"

void func_080041A0(u32 arg0, u32 arg1) {
    *(volatile u8 *)0x0300068A = arg0;
    *(volatile u8 *)0x0300068B = arg1;
}
