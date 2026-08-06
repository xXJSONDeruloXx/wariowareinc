#include "global.h"

extern void func_080338A4(void);
extern void func_08033C3C(s32);
extern void func_08033BAC(s32);

void func_08033D10(s32 arg0) {
    func_080338A4();
    func_08033C3C(arg0);
    func_08033BAC(arg0);
}
