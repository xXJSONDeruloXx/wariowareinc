#include "global.h"

u8 func_080039B4(u32 *counter) {
    *counter -= 1;
    return *(u8 *)(*counter);
}