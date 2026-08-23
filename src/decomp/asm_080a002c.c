#include "global.h"
#include "sound.h"

extern void set_soundplayer_volume(struct SoundPlayer *, u16);

void func_080A002C(u16 arg0) {
    u32 value;
    struct SoundPlayer *player;

    value = arg0;
    player = gBeatscriptScene.musicPlayer;
    value = (u16)value;
    set_soundplayer_volume(player, value);
}
