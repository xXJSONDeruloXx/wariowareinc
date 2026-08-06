#include "global.h"

void func_08035FEC(void *arg0, u16 arg1, u16 arg2) {
    u8 *base = (u8 *)arg0 + 0x90;

    *(u16 *)base = arg2;
    *(u16 *)(base + 2) = arg1;
    *(u16 *)(base - 4) = 0;
}
