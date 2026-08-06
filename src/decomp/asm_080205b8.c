#include "global.h"
#include "src/lib_sprite.h"

extern u32 get_current_mem_id(void);
extern void sprite_id_set_visible(struct SpriteHandler *, u32, u16);

void func_080205B8(void) {
    sprite_id_set_visible(gSpriteHandler, get_current_mem_id(), 0);
}
