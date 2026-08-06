#include "global.h"

extern u8 D_03000684;
extern void func_08003FD0(s32);

void func_080041B4(void) {
    if (D_03000684 != 0) {
        func_08003FD0(0);
    }
}
