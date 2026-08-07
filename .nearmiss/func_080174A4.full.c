#include "global.h"
#include "scenes.h"

extern void func_0800A280(s32);

void func_080174A4(u32 unused, u32 arg1) {
    if (arg1 != 0) {
        func_0800A280(0);
    } else {
        u8 *scene;

        scene = (u8 *)gCurrentSceneVariable;
        scene[4] &= (u8)-2;
    }
}
