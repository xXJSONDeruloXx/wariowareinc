#include "global.h"

struct Func0807DC6CInput {
    u8 pad0[4];
    s32 x;
    s32 y;
};

struct Func0807DC6COutput {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
};

void func_0807DC6C(struct Func0807DC6CInput *input,
                   struct Func0807DC6COutput *output) {
    s32 x;
    s32 y;

    x = input->x >> 8;
    y = input->y >> 8;
    output->left = x - 6;
    output->right = x + 6;
    output->top = y - 0xC;
    output->bottom = y;
}
