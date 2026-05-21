#include "global.h"

void func_08003FB8(void);

void func_0800418C(void) {
    *(volatile u8 *)0x03000684 = 0;
    func_08003FB8();
}
