#include "global.h"
#include "scenes.h"
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

void func_08011D0C(s32 arg0) {
    struct MainMenuPositionTable *table;
    struct MainMenuSceneSprites *scene;
    struct Vector2 *position;
    s16 sceneSprite;

    table = (struct MainMenuPositionTable *)&D_083AA294;
    position = table->positions[arg0 * 4];
    scene = (struct MainMenuSceneSprites *)gCurrentSceneData;
    sceneSprite = scene->spriteIds[arg0];
    run_func_after_task(
        (s32)func_0800C110(sceneSprite, position->x, position->y, -0x20,
                           position->y, 0xB4),
        (TaskFinalFunc)(func_080119EC + 1), 0);
}
