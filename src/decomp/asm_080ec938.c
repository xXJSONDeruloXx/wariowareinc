#include "global.h"
#include "scenes.h"
extern void set_soundplayer_pitch(void*,s32);
extern void func_08002038(void*,s32);
struct S{u8 pad[0x28];void*player;};
void func_080EC938(void){struct S*s=(struct S*)gCurrentSceneVariable;set_soundplayer_pitch(s->player,-0x100);s=(struct S*)gCurrentSceneVariable;func_08002038(s->player,0x80);}
