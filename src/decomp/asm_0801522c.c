#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void *gCurrentSceneData;
extern u8 D_083A4A2C;
extern void func_0800A240(void *, u32, u32, u32);
extern void func_08005600(void *, u32, void *, s16 *);
extern void mem_heap_dealloc(u32);

typedef struct Func0801522CScene {
    u8 padding4[4];
    u32 field4;
    u8 padding8[0xD6];
    u8 flags;
    u8 paddingDF[0x9D];
    u32 field17C;
    u8 padding180[0x14];
    s16 *field194;
    void *field198;
    void *field19C;
} Func0801522CScene;

void func_0801522C(void) {
    Func0801522CScene **base;
    Func0801522CScene *scene;
    void *asset;
    u8 *flagPtr;
    u32 value;
    u32 mask;

    asset = &D_083A4A2C;
    base = (Func0801522CScene **)&gCurrentSceneData;
    scene = *base;
    func_0800A240(asset, scene->field17C, 0, 0);
    func_08005600(gSpriteHandler, (*base)->field4, (*base)->field19C, (*base)->field194);
    mem_heap_dealloc((u32)(*base)->field194);
    mem_heap_dealloc((u32)(*base)->field198);
    mem_heap_dealloc((u32)(*base)->field19C);
    flagPtr = (u8 *)*base;
    flagPtr += 0xDE;
    value = *flagPtr;
    mask = 0x40;
    value |= mask;
    *flagPtr = value;
}
#endif
