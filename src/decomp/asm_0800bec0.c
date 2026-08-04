#if __INCLUDE_LEVEL__ > 0
#include "include/global.h"
#include "include/types.h"
#include "include/scenes.h"

u32 func_0800BEC0(void) {
    s32 value;

    value = ((u8 *)gCurrentSceneData)[0x195];
    switch (value) {
    case -10:
        goto zero;
    case 1:
    case 2:
    case 3:
        goto one;
    case 4:
        goto two;
    default:
        goto zero;
    }
one:
    return 1;
two:
    return 2;
zero:
    return 0;
}
#endif
