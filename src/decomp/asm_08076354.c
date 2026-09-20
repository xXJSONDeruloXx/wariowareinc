#include "global.h"
#include "scenes.h"
extern void func_080762DC(void);
struct S{u8 pad[0x10];s32 counter;u8 pad2;u8 enabled;};
void func_08076354(void){struct S*s=(struct S*)gCurrentSceneVariable;s->counter++;if(s->enabled&&s->counter>0x3C)func_080762DC();}
