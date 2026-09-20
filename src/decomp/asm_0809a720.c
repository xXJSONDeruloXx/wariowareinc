#include "global.h"
#include "types.h"
struct V21{u8 padding[0x21];u8 value;}; extern void func_0800C7FC(void *); extern void func_0800C9A4(s32); extern void func_0800A128(s32); extern u8 D_083FCB5C;
void func_0809A720(void){ struct V21 *v=(struct V21*)gCurrentSceneVariable; v->value=2; func_0800C7FC(&D_083FCB5C); func_0800C9A4(0x14); func_0800A128(1); }
