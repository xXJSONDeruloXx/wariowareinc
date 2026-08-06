#include "global.h"
#include "types.h"

void func_080721BC(u16 *dest, u16 *src, s32 count) {
    while (count != 0) {
        *dest = *src;
        src += 1;
        dest += 1;
        count -= 1;
    }
}
