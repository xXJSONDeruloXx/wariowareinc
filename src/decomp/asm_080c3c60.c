#include "global.h"
#include "scenes.h"
extern void scene_set_current_thread(u32);
struct S{u8 pad[0x90];s32 flag;};
void func_080C3C60(void){struct S*s;scene_set_current_thread(1);s=(struct S*)gCurrentSceneVariable;if(s->flag==0)s->flag=1;}
