#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

struct VarA2 {u8 padding[0xA2]; u8 flag;}; extern void func_080BA7C0(void); extern void func_080BA81C(void);
void func_080BA8A8(void){ struct ScenePhaseState *s=(struct ScenePhaseState*)gCurrentSceneData; struct VarA2 *v; if(s->phase==1){ func_080BA7C0(); v=(struct VarA2*)gCurrentSceneVariable; if(v->flag==1) func_080BA81C(); } }
