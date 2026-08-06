#include "global.h"
#include "types.h"

void func_080721A0(u8 *dest, u8 *src, s32 count) {
    while (count != 0) {
        *dest = *src;
        src += 1;
        dest += 1;
        count -= 1;
    }
}
