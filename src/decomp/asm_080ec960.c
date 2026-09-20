#include "global.h"
#include "types.h"
struct V28{u8 padding[0x28];void *player;}; extern void set_soundplayer_pitch(void *,s32); extern void func_08002038(void *,s32);
void func_080EC960(void){ struct V28 *v; v=(struct V28*)gCurrentSceneVariable; set_soundplayer_pitch(v->player,0); v=(struct V28*)gCurrentSceneVariable; func_08002038(v->player,0x100); }
