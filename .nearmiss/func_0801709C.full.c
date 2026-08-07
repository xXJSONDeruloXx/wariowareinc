#include "global.h"
#include "src/lib_sprite.h"
#include "src/scenes/gameplay.h"

extern void sprite_id_and_attr(struct SpriteHandler *, u32, u32);
extern void sprite_id_orr_attr(struct SpriteHandler *, u32, u32);

void func_0801709C(void) {
    struct GameplayData *data;

    data = gCurrentSceneData;
    sprite_id_and_attr(gSpriteHandler, 1, ~data->unk278);
    sprite_id_orr_attr(gSpriteHandler, 1, data->unk278 & data->unk274);
}
