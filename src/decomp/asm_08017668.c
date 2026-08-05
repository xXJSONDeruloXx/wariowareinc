#include "global.h"

extern void func_0800C7A4(u32);

void func_08017668(void) {
    u32 i;

    for (i = 0; i < 4; i++) {
        func_0800C7A4(i + 4);
    }
}
