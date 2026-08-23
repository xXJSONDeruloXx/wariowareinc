#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"

extern void sprite_set_x_y(void *, s32, s32, s32);

typedef struct {
    u8 padding2D0[0x2D0];
    s16 field2D0;
} Func0800E800Scene;

void func_0800E800(u32 arg0, u32 arg1) {
    void *handler;
    Func0800E800Scene *scene;
    s32 id;

    handler = (void *)gSpriteHandler;
    scene = (Func0800E800Scene *)gCurrentSceneData;
    id = scene->field2D0;
    sprite_set_x_y(handler, id, (s16)arg0, (s16)arg1);
}
