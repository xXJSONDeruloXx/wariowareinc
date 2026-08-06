#include "global.h"

extern void func_080F2F64(void *, u32, u16);

void set_soundplayer_volume(u32 arg0, u32 arg1) {
    u32 value;

    arg1 <<= 16;
    value = arg1 >> 16;
    if (arg0 != 0) {
        func_080F2F64((void *)arg0, 0xFFFF, (u16)value);
    }
}
