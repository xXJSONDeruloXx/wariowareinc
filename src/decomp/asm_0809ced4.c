#include "global.h"
#include "types.h"

void stop_soundplayer(u32);

void func_0809CED4(void) {
    u8 *p = (u8 *)gCurrentSceneVariable;
    stop_soundplayer(*(u32 *)(p + (0x8C << 1)));
}
