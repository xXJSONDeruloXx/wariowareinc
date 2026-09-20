#include "global.h"
#include "types.h"
#include "scenes.h"

struct ScenePhaseState { u8 padding[0x173]; u8 phase; };

extern void func_0808811C(void); extern void func_080882B4(void); extern void func_0808828C(void);
void func_0808844C(void){ struct ScenePhaseState *s; s=(struct ScenePhaseState*)gCurrentSceneData; if(s->phase==1){ func_0808811C(); func_080882B4(); } s=(struct ScenePhaseState*)gCurrentSceneData; if(s->phase>1){ func_080882B4(); func_0808828C(); } }
