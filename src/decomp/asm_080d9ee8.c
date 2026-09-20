#include "global.h"
#include "types.h"
struct V144{u8 padding[0x144];u8 base;}; extern void func_080D9F10(void *); extern void func_080D9F50(void *); extern void func_080D9FE8(void *);
void func_080D9EE8(void){ struct V144 *v=(struct V144*)gCurrentSceneVariable; void *p=&v->base; func_080D9F10(p); func_080D9F50(p); func_080D9FE8(p); }
