#include "global.h"

u8 func_0805F08C(u32 arg0) {
    u8 *p;

    if (arg0 > 0xFFU) {
        return 0x88;
    }
    p = (u8 *)gCurrentSceneVariable;
    p += 0x90;
    p += arg0;
    return *p;
}
