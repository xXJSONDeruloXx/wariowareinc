#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern u8 D_03000E70;
extern s32 sprite_is_invalid(void *, s16);
extern void sprite_remove_z_link(void *, s16);
extern void sprite_update_z_link(void *, s16);

void sprite_set_z(struct SpriteHandler *handler, s32 idArg, s32 zArg) {
    u16 z;
    s16 id;

    z = (u16)zArg;
    D_03000E70 = SPRITE_OPERATION_SET_Z;
    id = (s16)idArg;
    if (sprite_is_invalid(handler, id)) {
        return;
    }
    if (handler->sprites[id].zDepth == z) {
        return;
    }
    sprite_remove_z_link(handler, id);
    handler->sprites[id].zDepth = z;
    sprite_update_z_link(handler, id);
}
