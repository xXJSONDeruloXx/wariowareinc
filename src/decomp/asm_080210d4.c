#include "global.h"

extern u16 gCurrentKeys;
extern void func_08009EE4(u32);
extern void func_08021AB0(void);
extern void func_08021E48(void);
extern void func_08021EB8(void);
extern void func_08022090(void);

void func_080210D4(void) {
    func_08021AB0();
    func_08021E48();
    func_08021EB8();
    func_08022090();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
