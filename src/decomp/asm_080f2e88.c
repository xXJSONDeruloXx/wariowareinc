#include "global.h"
#include "sound.h"
extern void func_080F17D8(struct MidiBus *);
void func_080F2E88(struct SoundPlayer *player, u8 paused) {
    player->isPaused = paused;
    if (paused != 0)
        func_080F17D8(player->midiBus);
}
