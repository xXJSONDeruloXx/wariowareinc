#include "global.h"

struct CompressedData;

extern s32 start_new_task(u16 memID, void *task, void *stackArgs, void *arg3, u32 arg4);
extern u8 D_083A4B48;

s32 start_new_texture_loader(u16 memID, struct CompressedData **textureList) {
    return start_new_task(memID, &D_083A4B48, textureList, 0, 0);
}
