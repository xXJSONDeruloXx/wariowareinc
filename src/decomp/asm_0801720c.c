#include "global.h"

extern void func_08000778(u32);
extern u32 func_080007C0();
extern void func_0800A270(void);
extern void func_0800A3A4(s32);

void func_0801720C(u32 arg0) {
    if (func_080007C0() != 0) {
        func_0800A3A4(1);
    } else {
        func_0800A3A4(0);
        func_08000778(arg0);
        func_0800A270();
    }
}
