#include "global.h"
extern void scene_set_current_thread(u32);extern void sprite_set_visible(s32,s16,s32);
struct S{s16 sprite;u8 pad[0x16];s32 flag;};
void func_0807BA04(s32 a,s32 b,struct S*s){scene_set_current_thread(1);s->flag=0;sprite_set_visible(a,s->sprite,0);}
