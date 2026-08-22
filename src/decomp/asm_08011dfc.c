#if __INCLUDE_LEVEL__ > 0
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
    struct Func11DfcData **pp;
    struct Func11DfcData *scene;
    u32 count;
    u32 i;
    u8 zero;

    scene = SCENE;
    if (scene->unkF8 == 0)
        return;
    count = 1;
    if (scene->unkFA > 0x1B)
        count = 3;
    i = 0;
    if (i < count) {
        pp = (struct Func11DfcData **)&gCurrentSceneData;
        zero = 0;
        do {
            (*pp)->unkF4((*pp)->unkF9);
            if (++(*pp)->unkF9 >= (*pp)->unkFA) {
                (*pp)->unkF8 = zero;
                return;
            }
            i++;
        } while (i < count);
    }
}
#endif
