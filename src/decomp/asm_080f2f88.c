#include "global.h"
typedef struct{void *song;u16 player;u16 pad;} Song;typedef struct{u8 pad[0xC];void *song;} Obj;extern Song song_header_table[];extern u32 D_08406348;extern Obj *D_08406354[];extern void func_080F2EEC(Obj*);
void func_080F2F88(u16 id){u32 i;void *song=song_header_table[id].song;for(i=0;i<=D_08406348;i++){Obj *o=D_08406354[i];if(o!=0&&o->song==song)func_080F2EEC(o);}}
