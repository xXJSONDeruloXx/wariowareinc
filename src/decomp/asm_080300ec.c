#include "global.h"
extern u8 D_083FD9BC;
extern void stop_sound(void*);
struct S{u8 pad[0x32C];void*p;};
void func_080300EC(struct S*s){if(s->p){stop_sound(&D_083FD9BC);s->p=0;}}
