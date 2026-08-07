#include "global.h"
#include "scenes.h"

struct Func080D906CScene {
    u8 pad0[4];
    u32 index;
    u8 pad8[0x10];
    u32 tableOffset;
};

extern u8 D_083E55E4[];
extern struct BeatscriptLocalData *gCurrentSceneVariable;

s8 func_080D906C(void) {
    struct Func080D906CScene *scene;
    u32 index;
    u32 tableOffset;

    scene = (struct Func080D906CScene *)gCurrentSceneVariable;
    index = scene->index;
    tableOffset = scene->tableOffset;
    return (s8)D_083E55E4[tableOffset + (index * 3)];
}
