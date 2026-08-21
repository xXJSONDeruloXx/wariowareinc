#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"
#include "src/scenes/main_menu.h"

extern void func_08005E48(u32 *, u32, u32, u32, u32, u32, u32, u32);
extern void func_08012420(u32);

void func_0801216C(void) {
    u8 *flags;
    u8 val;
    u32 mask;

    flags = (u8 *)gCurrentSceneData + 0xDD;
    val = *flags;
    mask = 3;
    mask = -mask;
    mask = val & mask;
    *flags = mask;
    func_08005E48((u32 *)((u8 *)gCurrentSceneData + 0x7C), 0xF, 0, 0xE,
                  2, (u32)gMainMenu.unkD0, 0xF, 0);
    func_08012420(D_03006518.unk0);
}
#endif
