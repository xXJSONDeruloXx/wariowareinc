#include "global.h"
#include "src/code_08000f10.h"
#include "src/lib_sprite.h"
#include "scenes.h"

extern void sprite_handler_set_global_pause(struct SpriteHandler *, u16);

void func_08016DB8(void) {
    func_08000F74(NULL);
    sprite_handler_set_global_pause(gSpriteHandler, 1);
    *(u8 *)gCurrentSceneData = 0;
}
