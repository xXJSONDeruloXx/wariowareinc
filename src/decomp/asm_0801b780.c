#include "global.h"

extern void func_0801C368(void);
extern void func_08009EE4(u32);
extern u16 gCurrentKeys;

void func_0801B780(void) {
    func_0801C368();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
