#include "global.h"
#include "sound.h"

extern void set_soundplayer_volume(struct SoundPlayer *, u16);

void func_080A002C(u16 arg0) {
    register u16 value asm("r1") = arg0;
    struct SoundPlayer *player = gBeatscriptScene.musicPlayer;

    set_soundplayer_volume(player, value);
}
