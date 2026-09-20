#include "global.h"
struct Elem { u8 low2:2; u8 bit2:1; u8 rest:5; u8 pad[31]; };
extern struct Elem *D_030068E8;
void func_080F0E9C(u32 index, u32 value) { D_030068E8[index].bit2 = value; }
