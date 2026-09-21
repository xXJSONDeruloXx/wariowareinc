#include "global.h"
#include "scenes.h"
extern u16 D_03006520;extern void func_08021940(void);extern void func_08021A0C(void);struct S{u8 pad[0x30];s32 state;};
void func_08021AB0(void){if(D_03006520==0x14&&((struct S*)gCurrentSceneVariable)->state==1){func_08021940();func_08021A0C();}}
