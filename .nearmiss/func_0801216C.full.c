#include "src/scenes/main_menu.h"

extern void func_08005E48(void *, u32, u32, u32, u32, u32, u32, u32);
extern void func_08012420(u8);

void func_0801216C(void) {
    u8 *ptr;
    register u32 mask asm("r0") = 3;

    ptr = (u8 *)gCurrentSceneData + 0xDD;
    mask = -mask;
    *ptr &= mask;
    func_08005E48((u8 *)gCurrentSceneData + 0x7C, 0xF, 0, 0xE, 2,
                  *(u32 *)((u8 *)gCurrentSceneData + 0xD0), 0xF, 0);
    func_08012420(D_03006518.unk0);
}
