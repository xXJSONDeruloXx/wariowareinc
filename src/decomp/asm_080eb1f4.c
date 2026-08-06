#include "global.h"
#include "scenes.h"

extern void play_sound(struct SongHeader *);

void func_080EB1F4(s32 arg0) {
    if (((u8 *)gCurrentSceneData)[0x173] == 1) {
        play_sound((struct SongHeader *)arg0);
    }
}
