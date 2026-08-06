#include "global.h"

s32 func_08004770(u8 *arg0) {
    if (arg0[0] == 0x81 && arg0[1] == 0x40)
        return 1;
    if (arg0[0] == 0x20 && arg0[1] == 0x20)
        return 1;
    return 0;
}
