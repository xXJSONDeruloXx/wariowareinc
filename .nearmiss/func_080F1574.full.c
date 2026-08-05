#include "global.h"

extern u8 *D_030068E8;

u32 func_080F1574(u32 arg0) {
    return ((u32)*(u8 *)(D_030068E8 + (arg0 << 5)) << 31) >> 31;
}
