#include "scenes.h"

extern void func_08003D1C(void *);

void func_0800CE6C(void) {
    u8 *field = (u8 *)gCurrentSceneData + 0x1F4;
    if (((*field) << 0x1F) != 0) {
        func_08003D1C(field);
    }
}
