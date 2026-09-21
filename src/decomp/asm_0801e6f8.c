#include "global.h"
#include "scenes.h"
extern void mem_heap_dealloc(void*);
struct S{u8 pad0[0x10];u8 bit0:1;u8 rest:7;u8 pad1[0xA3];void*p;};
void func_0801E6F8(void){struct S **slot=(struct S**)&gCurrentSceneVariable;mem_heap_dealloc((*slot)->p);(*slot)->bit0=0;}
