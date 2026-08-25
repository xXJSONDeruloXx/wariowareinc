#include "global.h"

extern u16 *D_083A49E8;

u16 *func_080047D4(const u8 *arg0) {
    u16 **table;
    u32 index;

    table = &D_083A49E8;
    index = *arg0;
    index -= 'a';
    index <<= 1;
    return (u16 *)((u8 *)(*table) + index);
}
