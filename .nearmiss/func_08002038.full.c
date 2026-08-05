#include "global.h"
#include "types.h"

extern void set_soundplayer_speed(void *, u16);

void func_08002038(void *player, u32 speed) {
    if (player != NULL) {
        set_soundplayer_speed(player, (u16)speed);
    }
}
