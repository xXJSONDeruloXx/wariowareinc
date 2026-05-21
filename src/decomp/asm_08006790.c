#include "global.h"

void func_08006790(void *arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4) {
    *(u32*)((u8*)arg0 + 0x20) = arg1;
    *(u32*)((u8*)arg0 + 0x24) = arg2;
    *(u32*)((u8*)arg0 + 0x28) = arg3;
    *(u32*)((u8*)arg0 + 0x2C) = arg4;
}
