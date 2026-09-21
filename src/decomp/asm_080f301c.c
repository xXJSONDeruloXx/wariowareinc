#include "global.h"
#include "sound.h"
extern u32 func_080F2FFC(u16 tempo, u16 speed, u16 trackMap);
void set_soundplayer_speed(struct SoundPlayer *player, u16 speed) {
    u32 clocksPerFrame;
    player->volumeTrackMap = speed;
    clocksPerFrame = func_080F2FFC(player->midiTempo, speed, player->volumeB);
    if (clocksPerFrame == 0)
        clocksPerFrame = 1;
    player->clocksPerFrame = clocksPerFrame;
}
