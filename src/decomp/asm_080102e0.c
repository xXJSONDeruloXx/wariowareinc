#include "global.h"
extern u16 gPressedKeys;struct SongHeader;extern struct SongHeader D_083FBBD0;extern void func_080108D8(s32);extern void *play_sound(struct SongHeader*);
void func_080102E0(void){if(gPressedKeys&3){func_080108D8(2);play_sound(&D_083FBBD0);}}
