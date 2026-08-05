#include "global.h"

extern void func_08003DF4(void *arg0, void *arg1);
extern void func_0800BF0C(s32 arg0);

void func_08019A8C(void *arg0) {
    func_08003DF4(arg0, (void *)(VRAMBase + 0x8000));
    func_0800BF0C(0);
}
