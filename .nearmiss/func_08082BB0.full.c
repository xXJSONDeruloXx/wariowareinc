#include "global.h"
#include "src/lib_sprite.h"

struct Func08082BB0Scene {
    u8 pad0[0x10];
    s16 spriteId;
    u8 flag18;
};

extern struct BeatscriptLocalData *gCurrentSceneVariable;

void func_08082BB0(void) {
    ((struct Func08082BB0Scene *)gCurrentSceneVariable)->flag18 = 1;
    sprite_set_enable_updates(
        gSpriteHandler,
        ((struct Func08082BB0Scene *)gCurrentSceneVariable)->spriteId,
        0
    );
}
