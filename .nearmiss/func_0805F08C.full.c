#include "global.h"

u8 func_0805F08C(u32 arg0) {
    u8 *p = (u8 *)gCurrentSceneVariable;

    if (arg0 <= 0xFFU) {
        p += 0x90;
        p += arg0;
        return *p;
    }
    return 0x88;
}
