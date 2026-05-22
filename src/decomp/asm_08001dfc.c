#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern u8 D_03000118[];

s32 func_08001DFC(void) {
    u32 i;
    s32 counter = 0;
    
    for (i = 0; i <= 0x1F; i++) {
        if (D_03000118[i] != 0) {
            counter++;
        }
    }
    
    return counter;
}
#endif
