#include "global.h"
#include "src/audio.h"

void func_080B0760(void) {
    *(u16 *)((u8 *)gCurrentSceneVariable + 0x16) = 0x1E;
    play_sound((struct SongHeader *)0x083FC170);
}
