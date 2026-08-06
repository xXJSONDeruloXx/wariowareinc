#include "global.h"

extern void func_0801EA6C(void);
extern void func_0801ECA0(void);
extern void func_0801F2A0(void);
extern void func_08009EE4(u32);
extern u16 gCurrentKeys;

void func_0801E4EC(void) {
    func_0801EA6C();
    func_0801ECA0();
    func_0801F2A0();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
