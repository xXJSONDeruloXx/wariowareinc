#include "global.h"
extern void scene_set_music_pitch_env(s16 pitch);

void func_0800CA80(void)
{
    scene_set_music_pitch_env(0);
    gBeatscriptScene.unk1_b4 = 0;
    gBeatscriptScene.unk1_b5 = 0;
}
