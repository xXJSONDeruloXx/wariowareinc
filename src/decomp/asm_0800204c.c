#include "global.h"

void func_080F2F68(void);

void set_soundplayer_pitch(u8 a0) {
    if (a0 != 0) {
        func_080F2F68();
    }
}