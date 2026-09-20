#include "global.h"
#include "scenes.h"
extern void scene_set_current_thread(u32);
extern void sprite_set_visible(s32,s16,s32);
struct S{u8 pad[2];s16 sprite;};
void func_08037368(s32 a){scene_set_current_thread(1);sprite_set_visible(a,((struct S*)gCurrentSceneVariable)->sprite,0);}
