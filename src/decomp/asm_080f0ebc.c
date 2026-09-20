#include "global.h"
struct Elem { u8 low3:3; u8 bit3:1; u8 rest:4; u8 pad[31]; };
extern struct Elem *D_030068E8;
void func_080F0EBC(u32 index, u32 value) { D_030068E8[index].bit3 = value; }
