#include "global.h"
#include "sound.h"

extern void set_soundplayer_volume(struct SoundPlayer *, u16);

void func_080A002C(u16 arg0) {
    set_soundplayer_volume(gBeatscriptScene.musicPlayer, arg0);
}
