#include "global.h"
#include "scenes.h"
extern void func_0800A280(s32);
struct S{u8 pad[25];u8 bit0:1;u8 rest:7;};
void func_08022CF4(s32 a,s32 b){if(b)func_0800A280(0);else ((struct S*)gCurrentSceneVariable)->bit0=0;}
