#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern u8 D_03000E70;
extern s32 sprite_is_invalid(void *, s16);
extern void sprite_remove_z_link(void *, s16);
extern void sprite_update_z_link(void *, s16);

void sprite_set_x_y_z(struct SpriteHandler *handler, s32 idArg, s32 xArg, s32 yArg, s32 zArg) {
    u16 x;
    u16 y;
    u16 z;
    s16 id;

    x = (u16)xArg;
    y = (u16)yArg;
    z = (u16)zArg;
    D_03000E70 = SPRITE_OPERATION_SET_XYZ;
    id = (s16)idArg;
    if (!sprite_is_invalid(handler, id)) {
        handler->sprites[id].xPos = x;
        handler->sprites[id].yPos = y;
        if (handler->sprites[id].zDepth != z) {
            sprite_remove_z_link(handler, id);
            handler->sprites[id].zDepth = z;
            sprite_update_z_link(handler, id);
        }
    }
}
#endif
