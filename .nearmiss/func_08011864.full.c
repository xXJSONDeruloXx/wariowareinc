#include "src/scenes/main_menu.h"

extern void func_080140C0(void);

void func_08011864(u32 arg0) {
    u8 *var_r1;
    u8 var_r0;
    s32 var_r2;

    switch (arg0) {
    default:
        var_r1 = (u8 *)gCurrentSceneData + 0xDD;
        var_r0 = *var_r1;
        var_r2 = 4;
        goto block_6;
    case 1:
        var_r1 = (u8 *)gCurrentSceneData + 0xDD;
        var_r0 = *var_r1;
        var_r2 = 0x10;
        goto block_6;
    case 2:
        func_080140C0();
        return;
    }

block_6:
    *var_r1 = var_r0 | var_r2;
}
