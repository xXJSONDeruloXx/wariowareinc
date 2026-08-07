#include "global.h"
#include "src/audio.h"
#include "src/lib_sprite.h"

extern u32 D_083FF67C;
extern void func_08016B88(void *, u32, struct SongHeader *);

void func_08016B4C(void *unused, u32 id, struct SongHeader *sound) {
    s16 sprite_id;

    sprite_id = (s16)(u16)id;
    play_sound(sound);
    sprite_set_callback_cel(gSpriteHandler, sprite_id, 7);
    sprite_set_callback(gSpriteHandler, sprite_id, (void *)func_08016B88, (u32)&D_083FF67C);
}
