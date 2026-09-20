#include "global.h"
#include "types.h"
extern u16 gCurrentKeys; extern void func_0801B7E0(void); extern void func_0801B908(void); extern void func_0801BADC(void); extern void func_0801BDEC(void); extern void func_0801AFBC(void); extern void func_0801B1A8(void); extern void func_08009EE4(s32);
void func_0801AD6C(void){ func_0801B7E0(); func_0801B908(); func_0801BADC(); func_0801BDEC(); func_0801AFBC(); func_0801B1A8(); func_08009EE4((gCurrentKeys>>8)&1); }
