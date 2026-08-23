#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"
#include "src/task_pool.h"

extern void *func_0800C110();
extern void func_080119EC(void);
extern u8 D_083AA294;

struct MainMenuPositionTable {
    u8 pad0[0xC];
    struct Vector2 *positions[1];
};

struct MainMenuSceneSprites {
    u8 pad0[0x3A];
    s16 spriteIds[1];
};

void func_08011CA4(s32 arg0) {
    struct MainMenuPositionTable *table;
    struct Vector2 *position;
    s32 sceneOffset;

    sceneOffset = arg0 * 2;
    sprite_set_visible(gSpriteHandler,
                       *(s16 *)((u8 *)gCurrentSceneData + 0x3A + sceneOffset), 1);
    table = (struct MainMenuPositionTable *)&D_083AA294;
    position = table->positions[arg0 * 4];
    run_func_after_task(
        (s32)func_0800C110(
            *(s16 *)((u8 *)gCurrentSceneData + 0x3A + sceneOffset), -0x20,
            position->y, position->x,
                           position->y, 0xB4),
        (TaskFinalFunc)(func_080119EC + 1), 0);
}
