#include "global.h"
#include "types.h"

struct Func0801B174Scene {
    u8 pad0[0x19];
    u8 flags;
    u8 pad1[0xD4];
    u16 fieldEE;
    u16 fieldF0;
};

void func_0801B174(u16 arg0) {
    struct Func0801B174Scene *scene;
    u32 value;
    u32 flags;

    value = arg0;
    value <<= 16;
    value >>= 16;
    scene = (struct Func0801B174Scene *)gCurrentSceneVariable;
    scene->fieldF0 = value;
    scene->fieldEE = value;
    flags = scene->flags;
    flags |= 2;
    scene->flags = flags;
}
