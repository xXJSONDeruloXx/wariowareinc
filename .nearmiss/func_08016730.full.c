#include "global.h"
#include "src/scenes/title.h"

extern u16 D_03006520;
extern u16 get_current_mem_id(void);
extern void func_08009EE0_stub(u32);

void func_08016730(void) {
    u8 *scene;
    u32 sprite_id;

    sprite_id = func_080042F4(get_current_mem_id(), D_083ADADC, 0x300, 4, 0x200, 0x40);
    scene = (u8 *)gCurrentSceneData;
    *(u32 *)(scene + 4) = sprite_id;
    *(u8 *)(scene + 0x10) = (u8)(-2 & *(u8 *)(scene + 0x10));
    *(u16 *)(scene + 0x38) = 1;
    *(u16 *)(scene + 0x3A) = 0;
    D_03006520 = 0x270F;
    *(u8 *)(scene + 8) = 0;
    func_08009EE0_stub(1);
}
