#include "scenes.h"

void func_080DF224(void *, void *, u16);

void func_080B2724(void) {
    func_080DF224((u8 *)gCurrentSceneVariable + (0xE2 << 1),
                  (void *)0x083FBAA4,
                  *(u16 *)((u8 *)gCurrentSceneData + 0x16));
}
