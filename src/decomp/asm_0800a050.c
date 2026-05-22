#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

u8 func_0800A050(void) {
    u8 *ptr;
    ptr = (u8 *)gCurrentSceneData;
    return ptr[0x173];
}
#endif
