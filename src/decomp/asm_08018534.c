#include "src/lib_sprite.h"

extern u32 get_current_mem_id(void);
extern void sprite_id_set_visible(struct SpriteHandler *, u32, u16);

void func_08018534(void) {
    struct SpriteHandler *handler = gSpriteHandler;
    sprite_id_set_visible(handler, get_current_mem_id(), 0);
}
