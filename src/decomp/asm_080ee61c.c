#include "global.h"

/* BIOS Div consumes the incoming r0/r1 ABI values and returns the quotient in r0. */
s32 func_080EE61C(s32 a0, s32 a1) {
    return __builtin_swi_div();
}
