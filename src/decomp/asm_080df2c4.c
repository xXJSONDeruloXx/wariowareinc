#include "global.h"

struct SoundPlayer;
extern void set_soundplayer_pitch(struct SoundPlayer *arg0, u32 arg1);

void func_080DF2C4(struct SoundPlayer *arg0, u16 *arg1) {
    *arg1 -= 0xF;
    set_soundplayer_pitch(arg0, *(s16 *)arg1);
}
