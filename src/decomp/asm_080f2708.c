#include "global.h"
typedef struct{u8 pad[0xF];u8 value;u8 rest[0x10];} Entry; typedef struct{u8 pad0[0x14];u8 count:5;u8 rest:3;u8 pad15[3];Entry *entries;} Context;
void func_080F2708(Context *c,u8 value){u32 i;for(i=0;i<c->count;i++)c->entries[i].value=value;}
