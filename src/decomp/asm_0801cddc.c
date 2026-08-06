#include "global.h"

extern void func_0801D5A4(void);
extern void func_0801D710(void);
extern void func_08009EE4(u32);
extern u16 gCurrentKeys;

void func_0801CDDC(void) {
    func_0801D5A4();
    func_0801D710();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
