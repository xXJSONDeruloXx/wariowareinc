#include "global.h"

extern s16 func_080D6BA0(void);

s32 func_080D6D28(void) {
    s16 value = func_080D6BA0();

    if (value > 0x80) {
        return 2;
    }
    return 0x1A;
}
