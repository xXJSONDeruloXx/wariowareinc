#include "global.h"
#include "scenes.h"
struct Var{u8 pad[0x2E];s16 value;};extern u8 D_083B4D18;extern void scene_set_current_thread(u32);extern void func_08006CE8(s32,void*,s32,s16);
void func_0801CBB0(void){scene_set_current_thread(0);func_08006CE8(0,&D_083B4D18,0x20,((struct Var*)gCurrentSceneVariable)->value);}
