#include "global.h"

extern u8 D_03000E80[];
extern u16 D_03000E88[];
extern u16 D_03000E90[];

void func_080F2894(s32 arg0) {
    D_03000E80[arg0] = 1;
    D_03000E88[arg0] = 0xFFFF;
    D_03000E90[arg0] = 0xFFFF;
}
