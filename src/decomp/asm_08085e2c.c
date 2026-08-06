#include "scenes.h"

extern void func_08085D64(void);

void func_08085E2C(void) {
    if (((u8 *)gCurrentSceneData)[0x173] == 1) {
        func_08085D64();
    }
}
