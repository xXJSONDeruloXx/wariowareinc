#include "global.h"

extern void set_soundplayer_speed(void *, u16);

void func_08002038(u32 arg0, u32 arg1) {
    u16 value = (u16)arg1;

    if (arg0 != 0) {
        set_soundplayer_speed((void *)arg0, value);
    }
}
