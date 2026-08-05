#include "global.h"
#include "src/lib_sprite.h"

extern void sprite_handler_set_global_pause(struct SpriteHandler *, u16);

void func_08003228(void *arg0, u32 arg1) {
    *(u32 *)((u8 *)arg0 + 0x10) = arg1;
    sprite_handler_set_global_pause(*(struct SpriteHandler **)arg0, (u16)arg1);
}
