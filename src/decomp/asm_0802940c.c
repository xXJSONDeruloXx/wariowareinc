#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

extern void func_08029018(void *); extern void func_08029218(void *);
void func_0802940C(void *arg0){ struct ScenePhaseState *s; s=(struct ScenePhaseState*)gCurrentSceneData; if(s->phase!=0) func_08029018(arg0); s=(struct ScenePhaseState*)gCurrentSceneData; if(s->phase==1) func_08029218(arg0); }
