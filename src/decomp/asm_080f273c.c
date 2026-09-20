#include "global.h"
typedef struct{u8 a;u8 b;u16 rate;u8 e;u8 f;u8 x6;u8 x7;u32 x8;} Obj;
void func_080F273C(Obj *o,u8 a,u8 b,u8 div,u8 e,u8 f){o->x6=0;o->x7=0;o->x8=0;o->a=a;o->b=b;o->rate=0x10000/div;o->e=e;o->f=f;}
