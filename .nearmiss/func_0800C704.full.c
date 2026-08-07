#include "global.h"

extern void func_0800C61C(u32, u32);

void func_0800C704(u32 arg0, const void *arg1) {
    const u32 *cursor = (const u32 *)arg1;

    for (;;) {
        u32 value = *cursor++;
        if (value == 0) {
            break;
        }
        func_0800C61C(arg0, value);
    }
}
