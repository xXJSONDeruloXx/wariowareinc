#include "global.h"

void func_0801BEA8(void) {
    u8 *data = (u8 *)gCurrentSceneVariable;
    u8 value = data[0x18];
    value = (u8)((-0x3D & value) | 0x14);
    data[0x18] = value;
}
