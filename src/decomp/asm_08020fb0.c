#include "global.h"

extern void func_08021338(void);
extern void func_08021540(void);
extern void func_08021748(void);
extern void func_08009EE4(u32);
extern u16 gCurrentKeys;

void func_08020FB0(void) {
    func_08021338();
    func_08021540();
    func_08021748();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
