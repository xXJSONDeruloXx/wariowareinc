#include "global.h"

extern void func_0801F370(void);
extern void func_0801F620(void);
extern void func_08009EE4(u32);
extern u16 gCurrentKeys;

void func_0801E918(void) {
    func_0801F370();
    func_0801F620();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
