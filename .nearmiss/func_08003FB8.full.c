#include "global.h"

extern u8 D_03000528;

void func_08003FB8(void) {
    D_03000528 = (u8)(-2 & D_03000528 & ~8);
}
