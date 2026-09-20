#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

struct Var4 {u8 padding[4]; s32 value;}; extern void func_08081B88(void); extern void func_08081A50(void); extern void func_08081DDC(void); extern void func_08081EAC(void);
void func_08081F94(void){ struct ScenePhaseState *s=(struct ScenePhaseState*)gCurrentSceneData; struct Var4 *v; if(s->phase==1){ v=(struct Var4*)gCurrentSceneVariable; if(v->value>0) func_08081B88(); } func_08081A50(); func_08081DDC(); func_08081EAC(); }
