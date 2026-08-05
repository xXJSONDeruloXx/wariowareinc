#include "global.h"

void func_08035FEC(u8 *arg0, s16 arg1, s16 arg2) {
    u8 *ptr = arg0 + 0x90;
    *(u16 *)ptr = arg2;
    ptr += 2;
    *(u16 *)ptr = arg1;
    ptr -= 4;
    *(u16 *)ptr = 0;
}
