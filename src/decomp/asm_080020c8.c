#include "global.h"

void func_080F2F64(void);

void set_soundplayer_volume(u8 a0) {
    if (a0 != 0) {
        func_080F2F64();
    }
}