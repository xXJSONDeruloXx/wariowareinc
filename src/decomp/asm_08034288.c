#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

struct Var34 {u8 padding[0x34]; s32 state;}; extern void func_08034100(void); extern void func_08033F74(void);
void func_08034288(void){ struct ScenePhaseState *s=(struct ScenePhaseState*)gCurrentSceneData; struct Var34 *v; if(s->phase==1){ v=(struct Var34*)gCurrentSceneVariable; if(v->state==1) func_08034100(); else func_08033F74(); } }
