#include "global.h"

extern void func_08022B28(void);
extern void func_08009EE4(u32);
extern u16 gCurrentKeys;

void func_080227F0(void) {
    func_08022B28();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
