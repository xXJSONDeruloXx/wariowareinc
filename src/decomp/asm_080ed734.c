#include "global.h"

extern void func_080ED1A8(u8, void *);

void func_080ED734(void *arg0) {
    u8 *base = (u8 *)arg0;

    func_080ED1A8(base[0x12], arg0);
    base[0x11] = 3;
}
