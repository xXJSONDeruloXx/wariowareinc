#include "global.h"
#include "src/audio.h"

void func_080C691C(void) {
    u8 *base = (u8 *)gCurrentSceneVariable;
    base += (0xCC << 1);
    *base = 0;
    play_sound((struct SongHeader *)0x083FF348);
}
