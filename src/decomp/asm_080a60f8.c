#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

struct ScenePhaseGate {u8 padding[0x173]; u8 phase; u8 gap[4]; u16 gate;}; extern void func_080A60AC(void); extern void func_080A5B2C(void); extern void func_080A5BCC(void); extern void func_080A5E8C(void);
void func_080A60F8(void){ struct ScenePhaseGate *s=(struct ScenePhaseGate*)gCurrentSceneData; if(s->phase==1 && s->gate==0){ func_080A60AC(); func_080A5B2C(); func_080A5BCC(); func_080A5E8C(); } }
