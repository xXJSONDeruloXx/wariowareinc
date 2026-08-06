#include "global.h"
#include "src/audio.h"

void func_080B29C8(void) {
    play_sound((struct SongHeader *)0x083FC15C);
    *(u8 *)((u8 *)gCurrentSceneVariable + 0x1B1) = 1;
}
