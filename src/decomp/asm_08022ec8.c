#include "global.h"

extern void func_080233B8(void);
extern void func_08009EE4(u32);
extern u16 gCurrentKeys;

void func_08022EC8(void) {
    func_080233B8();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
