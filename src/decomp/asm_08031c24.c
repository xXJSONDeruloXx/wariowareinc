#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

struct Obj74 {u8 padding[0x74]; s32 counter;}; extern void func_08031948(void *); extern void func_08031A5C(void *);
void func_08031C24(struct Obj74 *o){ struct ScenePhaseState *s; o->counter++; s=(struct ScenePhaseState*)gCurrentSceneData; if(s->phase<=1){ func_08031948(o); func_08031A5C(o); } }
