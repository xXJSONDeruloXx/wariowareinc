#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

struct SongHeader; extern struct SongHeader D_083FD4F8; extern void *play_sound(struct SongHeader *); extern void scene_set_current_thread(u32);
void func_0805172C(void){ struct ScenePhaseState *s; scene_set_current_thread(1); s=(struct ScenePhaseState*)gCurrentSceneData; if(s->phase==1) play_sound(&D_083FD4F8); }
