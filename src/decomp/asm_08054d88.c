#include "global.h"
#include "scenes.h"
extern void func_0800C7FC(s32);struct S{u8 pad[0x10];u8 flag;};
void func_08054D88(s32 a){struct S **slot=(struct S**)&gCurrentSceneVariable;if((*slot)->flag==0){func_0800C7FC(a);(*slot)->flag=1;}}
