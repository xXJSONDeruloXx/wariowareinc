#include "global.h"
#include "scenes.h"
#include "src/task_pool.h"

extern void *func_0800C110();
extern void func_080119EC(void);
extern u8 D_083AA0C4[];
extern u8 D_083AA294;

struct MainMenuPositionTable {
    u8 pad0[0xC];
    struct Vector2 *positions[1];
};

struct MainMenuSceneSprites {
    u8 pad0[0x3A];
    s16 spriteIds[1];
};

void func_08011BEC(s32 arg0) {
    struct MainMenuSceneSprites *scene;
    struct Vector2 *positionA;
    struct Vector2 *positionB;
    s16 sceneSprite;

    if (arg0 <= 8) {
        positionA = ((struct MainMenuPositionTable *)D_083AA0C4)->positions[arg0 * 4];
        positionB = ((struct MainMenuPositionTable *)&D_083AA294)->positions[arg0 * 4];
    } else {
        positionB = ((struct MainMenuPositionTable *)&D_083AA294)->positions[0];
        positionA = ((struct MainMenuPositionTable *)D_083AA0C4)->positions[arg0 * 4];
    }
    scene = (struct MainMenuSceneSprites *)gCurrentSceneData;
    sceneSprite = scene->spriteIds[arg0];
    run_func_after_task(
        (s32)func_0800C110(sceneSprite, positionA->x, positionA->y,
                           positionB->x, positionB->y, 0xB4),
        (TaskFinalFunc)(func_080119EC + 1), 0);
}
