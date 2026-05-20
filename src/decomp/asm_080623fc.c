#include "global.h"
#include "types.h"

void func_080623FC(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    *(u32 *)(p + (0xBD << 4)) = 0;
}
