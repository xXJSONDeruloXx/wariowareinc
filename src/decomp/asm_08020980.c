#include "global.h"
#include "scenes.h"
extern void func_08009EE0_stub(s32);extern u8 D_083BE2A4;extern void func_0800A3D0(void*);
struct S{u8 pad[0x18];u8 bit0:1;u8 rest:7;};
void func_08020980(void){func_08009EE0_stub(0);((struct S*)gCurrentSceneVariable)->bit0=0;func_0800A3D0(&D_083BE2A4);}
