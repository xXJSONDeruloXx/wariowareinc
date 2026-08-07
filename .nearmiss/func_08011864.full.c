#include "global.h"

struct Func08011864SceneV5 {
    u8 pad0[0xDD];
    u8 flags;
};

extern void *gCurrentSceneData;
extern void func_080140C0(void);

void func_08011864(u32 arg0) {
    switch (arg0) {
    case 0:
        ((struct Func08011864SceneV5 *)gCurrentSceneData)->flags |= 4;
        break;
    case 1:
        ((struct Func08011864SceneV5 *)gCurrentSceneData)->flags |= 0x10;
        break;
    case 2:
        func_080140C0();
        break;
    default:
        break;
    }
}
