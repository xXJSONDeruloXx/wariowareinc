#include "global.h"

void func_08003014(void *arg0, u32 arg1, u8 arg2, u8 arg3) {
    *(u32 *)arg0 = arg1;
    arg1 = 0;
    *(u8 *)((u8 *)arg0 + 4) = arg2;
    *(u8 *)((u8 *)arg0 + 5) = arg3;
    arg0 = (u8 *)arg0 + 8;
    *(u32 *)arg0 = arg1;
    *(u8 *)((u8 *)arg0 + 5) = arg1;
    *(u8 *)((u8 *)arg0 + 4) = arg1;
}
