#include "global.h"
#include "sound.h"
s32 func_080F2EB4(struct SoundPlayer *player) {
    u32 i;
    if (player->song == NULL)
        return FALSE;
    for (i = 0; i < player->usedTracks; i++) {
        if (player->midiReader[i].active)
            return TRUE;
    }
    return FALSE;
}
