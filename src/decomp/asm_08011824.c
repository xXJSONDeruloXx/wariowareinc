#if __INCLUDE_LEVEL__ > 0
#include "global.h"

/* gSpriteHandler, sprite_set_anim_cel, gCurrentSceneSpritePool: from lib_sprite.h via main_menu.h */
/* func_0800C7A4: declared by other included_stub files in this TU */
extern void func_0800C77C(u32);

void func_08011824(void) {
    func_0800C7A4(1);
    func_0800C7A4(2);
    func_0800C7A4(3);
    func_0800C7A4(0xA);
    sprite_set_anim_cel(gSpriteHandler, gCurrentSceneSpritePool[6], 0);
    func_0800C77C(6);
}
#endif
