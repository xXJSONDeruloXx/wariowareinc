#include "global.h"
#include "src/lib_sprite.h"

extern void func_08001B70(u32 arg0);

void func_08016FB0(void) {
    sprite_id_delete(gSpriteHandler, 1);
    func_08001B70(1);
}
