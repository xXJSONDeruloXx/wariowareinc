#include "global.h"

struct SoundPlayer;
extern void set_soundplayer_pitch(struct SoundPlayer *arg0, u32 arg1);

void func_080DF2D8(struct SoundPlayer *arg0, u16 *arg1) {
    *arg1 -= 3;
    set_soundplayer_pitch(arg0, *(s16 *)arg1);
}
