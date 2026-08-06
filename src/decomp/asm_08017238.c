#include "global.h"

extern void func_0800A3A4(s32);

void func_08017238(void) {
    if (*(u8 *)0x03003634 != 0) {
        func_0800A3A4(1);
    } else {
        func_0800A3A4(0);
    }
}
