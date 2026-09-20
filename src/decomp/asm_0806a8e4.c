#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

extern void func_0806A654(void); extern void func_0806A50C(void); extern void func_0806A118(void); extern void func_0806A7B8(void); extern s32 func_0806A73C(void); extern void func_0806A780(void); extern void func_0806A858(void);
void func_0806A8E4(void){ struct ScenePhaseState *s=(struct ScenePhaseState*)gCurrentSceneData; if(s->phase==1){ func_0806A654(); func_0806A50C(); func_0806A118(); func_0806A7B8(); if((u16)func_0806A73C()!=0) func_0806A780(); } func_0806A858(); }
