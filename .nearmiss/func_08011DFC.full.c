
#include "global.h"

extern void *gCurrentSceneData;

struct Func11DfcData {
    u8 pad0[0xF4];
    void (*unkF4)(u32);
    u8 unkF8;
    u8 unkF9;
    u8 unkFA;
};

#define SCENE ((struct Func11DfcData *)gCurrentSceneData)

void func_08011DFC(void) {
    struct Func11DfcData *p;
    u32 count;
    u32 i;

    p = SCENE;
    if (p->unkF8 == 0)
        return;
    count = 1;
    if (p->unkFA > 0x1B)
        count = 3;
    for (i = 0; i < count; i++) {
        SCENE->unkF4(SCENE->unkF9);
        if (++SCENE->unkF9 >= SCENE->unkFA) {
            SCENE->unkF8 = 0;
            return;
        }
    }
}
