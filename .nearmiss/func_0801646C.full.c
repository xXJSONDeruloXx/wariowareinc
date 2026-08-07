#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"
#include "scenes.h"

void func_0801646C(void) {
    u32 mask;
    u32 i;
    u32 offset;
    u8 *base;
    s16 id;
    struct SpriteHandler *handler;
    register u32 *scene_ref asm("r2");
    register u8 *scene asm("r3");

    scene_ref = (u32 *)&gCurrentSceneData;
    scene = (u8 *)*scene_ref;
    *(u16 *)(scene + 0x1BA) = 0x3C;
    {
        register u32 offset asm("r0");
        register u8 *store asm("r1");

        store = scene;
        offset = 0xDC;
        offset <<= 1;
        store += offset;
        *store = 1;
    }
    {
        register u32 base asm("r0");
        register u32 offset asm("r1");

        base = *scene_ref;
        offset = 0xDA;
        offset <<= 1;
        base += offset;
        mask = *(u32 *)base;
    }
    i = 0;
    do {
        if (((mask >> i) & 1) != 0) {
            handler = gSpriteHandler;
            base = (u8 *)gCurrentSceneData;
            offset = i << 1;
            base += 0x3A;
            base += offset;
            id = *(s16 *)base;
            sprite_set_anim_cel(handler, id, 0);
        }
        i++;
    } while (i <= 0x1B);
}
