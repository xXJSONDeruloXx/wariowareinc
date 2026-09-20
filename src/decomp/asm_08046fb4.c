#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

extern void func_08046D78(void); extern void func_08046E00(void); extern void func_08046E48(void); extern void func_08046F5C(void);
void func_08046FB4(void){ struct ScenePhaseState *s=(struct ScenePhaseState*)gCurrentSceneData; if(s->phase!=0){ if(s->phase==1) func_08046D78(); func_08046E00(); func_08046E48(); func_08046F5C(); } }
