#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u32 D_083A4BF0[];

s32 func_0800A430(s32 arg0) {
    u32 *entry;

    entry = D_083A4BF0;
    while (entry[0] != 0) {
        if (entry[0] == arg0) {
            return entry[1];
        }
        entry += 2;
    }
    return 0x8C;
}
#endif
