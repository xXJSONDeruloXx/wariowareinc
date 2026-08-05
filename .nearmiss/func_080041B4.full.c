#include "global.h"

extern void func_08003FD0(s32);
extern u8 D_03000684;

void func_080041B4(void) {
    if (D_03000684 != 0) {
        func_08003FD0(0);
    }
}
