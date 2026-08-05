#include "global.h"

extern void *func_08006184(u16, u32);

void func_080043A0(void *arg0, u32 arg1, u32 arg2) {
    *(u32 *)((u8 *)arg0 + 0x14) = arg1;
    *(u32 *)((u8 *)arg0 + 0x18) = (u32)func_08006184(*(u16 *)arg0, arg2);
}
