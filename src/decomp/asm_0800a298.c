#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/lib_sprite.h"

extern struct SpriteHandler *gSpriteHandler;
extern void *gCurrentSceneData;
extern void sprite_id_and_attr(struct SpriteHandler *, u32, u32);
extern void sprite_id_orr_attr(struct SpriteHandler *, u32, u32);

void func_0800A298(u32 arg0, u32 arg1) {
    u8 *data;
    sprite_id_and_attr(gSpriteHandler, 1, ~arg0);
    sprite_id_orr_attr(gSpriteHandler, 1, arg0 & arg1);
    data = gCurrentSceneData;
    *(u32 *)(data + (0x9D << 2)) = arg1;
    *(u32 *)(data + ((0x9D << 2) + 4)) = arg0;
}
#endif
