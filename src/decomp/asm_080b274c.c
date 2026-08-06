#include "scenes.h"

void func_080DF28C(u32, u16);

void func_080B274C(void) {
    func_080DF28C(*(u32 *)((u8 *)gCurrentSceneVariable + (0xE2 << 1)),
                  *(u16 *)((u8 *)gCurrentSceneData + 0x16));
}
