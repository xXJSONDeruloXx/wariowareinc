#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"

extern void func_0800BF7C(s32, u32, s16, s16, u32, u32, u32);
extern void *func_0800A240(void *, void *, void *, u32);
extern u32 get_current_mem_id(void);
extern void func_080055D4(u16, void *, void *, void *, s16 *);
extern u8 D_083A4A1C;
extern u8 D_083AB394;

struct Func08014CF8Scene {
    void *unk0;
    void *unk4;
    u8 pad8[0xD6];
    u8 flags;
    u8 padDF[0x8D];
    void *unk16C;
};

void func_08014CF8(void) {
    u8 flags;

    func_0800BF7C(1, 1, 0, 0, 0, 9, 1);
    func_0800A240(&D_083A4A1C,
                  ((struct Func08014CF8Scene *)gCurrentSceneData)->unk16C,
                  0, 0);
    func_080055D4(
        (u16)get_current_mem_id(), gSpriteHandler,
        ((struct Func08014CF8Scene *)gCurrentSceneData)->unk4,
                  &D_083AB394, gCurrentSceneSpritePool);
    flags = ((struct Func08014CF8Scene *)gCurrentSceneData)->flags;
    flags |= 0x20;
    ((struct Func08014CF8Scene *)gCurrentSceneData)->flags = flags;
}
