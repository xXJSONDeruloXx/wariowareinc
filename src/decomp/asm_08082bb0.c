#include "global.h"
#include "scenes.h"
extern s32 gSpriteHandler;extern void sprite_set_enable_updates(s32,s16,s32);struct S{u8 pad[0x16];s16 sprite;u8 flag;};
void func_08082BB0(void){struct S **slot=(struct S**)&gCurrentSceneVariable;(*slot)->flag=1;sprite_set_enable_updates(gSpriteHandler,(*slot)->sprite,0);}
