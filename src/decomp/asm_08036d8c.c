#include "global.h"
#include "scenes.h"
struct S{u8 pad[164];u32 mask;u8 gap[0x30];u32 active;};
void func_08036D8C(void){struct S*s=(struct S*)gCurrentSceneVariable;if((s->mask&s->active)==0)s->mask|=1;}
