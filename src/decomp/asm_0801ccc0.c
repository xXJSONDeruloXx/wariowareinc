#include "global.h"

extern void func_0801CFD8(void);
extern void func_0801D2F0(void);
extern void func_08009EE4(u32);
extern u16 gCurrentKeys;

void func_0801CCC0(void) {
    func_0801CFD8();
    func_0801D2F0();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
