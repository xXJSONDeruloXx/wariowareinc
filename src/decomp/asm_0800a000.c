#if __INCLUDE_LEVEL__ > 0
#include "global.h"

extern struct BeatscriptScene gBeatscriptScene;
extern void set_soundplayer_volume(struct SoundPlayer *, u16);

void func_0800A000(u16 arg0) {
    u8 *base = (u8 *)&gBeatscriptScene;
    u16 *dest = (u16 *)(base + 0x1C58);
    *dest = arg0;
    set_soundplayer_volume(gBeatscriptScene.musicPlayer, arg0);
}
#endif
