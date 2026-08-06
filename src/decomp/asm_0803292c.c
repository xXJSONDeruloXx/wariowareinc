#include "global.h"

extern void func_080323A4(s32);
extern void func_080325C0(void);
extern void func_080326FC(s32);
extern void func_0803280C(s32);

void func_0803292C(s32 arg0) {
    func_080325C0();
    func_080326FC(arg0);
    func_0803280C(arg0);
    func_080323A4(arg0);
}
