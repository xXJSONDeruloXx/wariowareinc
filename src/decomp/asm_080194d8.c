#include "global.h"

extern void func_0801955C(void);
extern void func_08019600(void);
extern void func_080199EC(void);
extern void func_08019790(void);
extern void func_08009EE4(u32);
extern u16 gCurrentKeys;

void func_080194D8(void) {
    func_0801955C();
    func_08019600();
    func_080199EC();
    func_08019790();
    func_08009EE4(((u16)gCurrentKeys >> 8) & 1);
}
