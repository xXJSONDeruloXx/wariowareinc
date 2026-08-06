#include "global.h"

void func_0806F0D4(void) {
    u8 *base = (u8 *)gCurrentSceneVariable;

    if (base[0x25] != 0) {
        *(u8 *)(base + 2) = 7;
    }
}
