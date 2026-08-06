#include "global.h"

void func_0801AF18(void) {
    u8 *data = (u8 *)gCurrentSceneVariable;
    u8 value = data[0x18];
    value = (u8)((-0x3D & value) | 8);
    data[0x18] = value;
}
