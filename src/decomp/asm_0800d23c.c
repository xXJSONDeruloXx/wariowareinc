#include "global.h"

extern void func_080043A0(void *arg0, void *arg1, u32 arg2);
extern void func_0800CFFC(void);

void func_0800D23C(void *arg0) {
    func_080043A0(arg0, (void *)((u32)func_0800CFFC + 1), 0x101);
}
