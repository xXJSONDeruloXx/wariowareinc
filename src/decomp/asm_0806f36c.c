#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

extern void func_0806F1E8(void); extern void func_0806ED68(void); extern s32 func_0806EC7C(void); extern void func_0800A0C4(s32); extern void func_0806F0C4(void);
void func_0806F36C(void){ struct ScenePhaseState *s=(struct ScenePhaseState*)gCurrentSceneData; if(s->phase==1){ func_0806F1E8(); func_0806ED68(); if((u16)func_0806EC7C()!=0){ func_0800A0C4(0); func_0806F0C4(); } } }
