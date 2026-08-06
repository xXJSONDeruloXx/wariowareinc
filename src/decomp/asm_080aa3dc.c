#include "global.h"

void func_080AA3DC(void) {
    u8 *ptr = (u8 *)gCurrentSceneVariable + (0xFF << 1);
    *ptr = (*ptr & 1) | 2;
}
