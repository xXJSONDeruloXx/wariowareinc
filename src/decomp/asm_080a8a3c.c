#include "global.h"

extern void func_080A898C(u32);

void func_080A8A3C(void) {
    u32 i;

    for (i = 0; i < 8; i++) {
        func_080A898C(i);
    }
}
