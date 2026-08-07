#include "global.h"
#include "src/audio.h"
#include "src/lib_sprite.h"

void func_08016BC4(void *unused, u32 id, struct SongHeader *sound) {
    s16 sprite_id;

    sprite_id = (s16)(u16)id;
    play_sound(sound);
    sprite_set_callback_cel(gSpriteHandler, sprite_id, -1);
}
