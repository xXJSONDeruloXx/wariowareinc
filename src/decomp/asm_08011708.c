#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern void *gCurrentSceneData;
extern s32 func_08011614(void);

s32 func_08011708(void) {
    u8 *base = gCurrentSceneData;
    s32 val = base[0xDF];
    val = val << 0x1D;
    if (val < 0) {
        return 0;
    }
    if (func_08011614() != 0) {
        return 0;
    }
    return 1;
}
#endif
