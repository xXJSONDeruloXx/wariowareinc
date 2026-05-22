#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

u16 func_0800A138(void) {
    u8 *ptr;
    ptr = (u8 *)gCurrentSceneData;
    return *(u16 *)(ptr + (0xBD << 1));
}
#endif
