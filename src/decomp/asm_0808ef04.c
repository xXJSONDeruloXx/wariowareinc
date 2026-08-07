#include "global.h"

struct Func0808EF04Scene {
    u8 pad0[0xE];
    u16 value;
};

struct Func0808EF04Output {
    s16 value;
    u8 pad2[2];
    u8 flag4;
    u8 flag5;
    u8 flag6;
};

extern struct BeatscriptLocalData *gCurrentSceneVariable;

void func_0808EF04(struct Func0808EF04Output *output) {
    struct Func0808EF04Scene *scene;

    scene = (struct Func0808EF04Scene *)gCurrentSceneVariable;
    output->value = (s16)((s32)(scene->value * 0x4B) >> 6);
    output->flag4 = 0;
    output->flag6 = 0;
    output->flag5 = 0;
}
