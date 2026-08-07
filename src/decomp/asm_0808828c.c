#include "global.h"
#include "src/lib_sprite.h"

struct Func0808828CScene {
    u8 pad0[2];
    s16 spriteId;
    u8 pad4[0xC];
    u8 flag10;
};

extern struct BeatscriptLocalData *gCurrentSceneVariable;

void func_0808828C(void) {
    ((struct Func0808828CScene *)gCurrentSceneVariable)->flag10 = 5;
    sprite_set_enable_updates(
        gSpriteHandler,
        ((struct Func0808828CScene *)gCurrentSceneVariable)->spriteId,
        1
    );
}
