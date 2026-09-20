#include "global.h"
typedef struct{u8 pad0[2];u8 value;u8 pad3[0x11];u8 count:5;u8 rest:3;} Context; extern void func_080F23F8(Context*,u32);
void func_080F26D8(Context *c,u8 value){u32 i;c->value=value;for(i=0;i<c->count;i++)func_080F23F8(c,i);}
