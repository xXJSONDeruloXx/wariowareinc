#include "global.h"
#include "src/scenes/gameplay.h"

void func_08025160(u16 arg0, u16 arg1) {
    u8 *p = (u8 *)D_03006524;
    *(u16 *)(p + 0x54) = arg0;
    *(u16 *)(p + 0x56) = arg1;
}
