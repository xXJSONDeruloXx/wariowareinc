#include "global.h"
struct Obj { u8 field0:5; u8 rest0:3; u8 pad1; u8 low5:5; u8 bit5:1; u8 rest2:2; u8 pad3; void *a; void *b; void *c; };
void func_080F3C7C(struct Obj *o, void *a, u32 field, void *b, u32 flag) { o->c=0; o->a=a; o->field0=field; o->b=b; o->bit5=flag; }
