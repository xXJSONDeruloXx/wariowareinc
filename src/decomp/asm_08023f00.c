#include "global.h"
#include "types.h"
extern u16 gCurrentKeys; extern void func_080240E0(void); extern void func_0802415C(void); extern void func_08024208(void); extern void func_0802426C(void); extern void func_08024450(void); extern void func_080244EC(void); extern void func_08009EE4(s32);
void func_08023F00(void){ func_080240E0(); func_0802415C(); func_08024208(); func_0802426C(); func_08024450(); func_080244EC(); func_08009EE4((gCurrentKeys>>8)&1); }
