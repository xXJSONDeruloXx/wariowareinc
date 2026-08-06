#include "scenes.h"

extern void func_08054FE0(void);

void func_080550C4(void) {
    if (((u8 *)gCurrentSceneData)[0x173] == 1) {
        func_08054FE0();
    }
}
