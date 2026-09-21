#include "global.h"
#include "scenes.h"
#include "sound.h"
struct SceneSoundState { u32 reserved[10]; struct SoundPlayer *player; };
extern void set_soundplayer_pitch(struct SoundPlayer *, s32);
extern void func_08002038(struct SoundPlayer *, u32);
void func_080EC910(void){ struct SceneSoundState *scene=(struct SceneSoundState *)gCurrentSceneVariable; set_soundplayer_pitch(scene->player,0x100); scene=(struct SceneSoundState *)gCurrentSceneVariable; func_08002038(scene->player,0x200); }
