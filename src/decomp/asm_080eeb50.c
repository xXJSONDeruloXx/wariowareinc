#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern u8 D_03000E70;
extern s32 sprite_is_invalid(void *, s16);
extern void sprite_set_anim_cel(struct SpriteHandler *, s16, s8);

void sprite_set_anim_progress(struct SpriteHandler *handler, s32 idArg, s32 progressArg) {
    u8 progress;
    s16 id;
    struct Sprite *sprite;
    struct Animation *it;
    s32 target;
    u32 index;
    u32 sum;

    progress = (u8)progressArg;
    D_03000E70 = SPRITE_OPERATION_SET_ANIM_PROGRESS;
    id = (s16)idArg;
    if (sprite_is_invalid(handler, id)) {
        return;
    }
    sprite = &handler->sprites[id];
    target = (progress * sprite->totalDuration) >> 8;
    it = sprite->animation;
    index = 0;
    sum = 0;
    while (1) {
        sum += it->duration;
        if (target < sum) {
            sprite_set_anim_cel(handler, id, (s8)index);
            return;
        }
        it++;
        index = ((index << 24) + 0x1000000) >> 24;
        if (it->cel == NULL) {
            return;
        }
    }
}
#endif
