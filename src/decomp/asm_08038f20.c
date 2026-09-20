#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

struct Var94 {u8 padding[0x94]; s32 counter;}; extern void func_08038DA8(void); extern void func_08038A44(void); extern void func_08038AF0(void);
void func_08038F20(void){ struct ScenePhaseState *s=(struct ScenePhaseState*)gCurrentSceneData; struct Var94 *v; if(s->phase<=1){ func_08038DA8(); func_08038A44(); func_08038AF0(); v=(struct Var94*)gCurrentSceneVariable; v->counter++; } }
