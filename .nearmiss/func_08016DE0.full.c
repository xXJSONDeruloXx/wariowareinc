#include "global.h"
#include "graphics.h"
#include "scenes.h"
#include "src/code_08000f10.h"
#include "src/scenes/gameplay.h"

extern void func_08003EB0(void);
extern void func_08002090(u32);

s32 func_08016DE0(void) {
    u8 state;
    u8 *scene;

    func_08003EB0();
    flush_graphics_buffer();
    trigger_pending_dma3();
    scene = (u8 *)gCurrentSceneData;
    state = scene[0];
    if (state == 1) {
        if (gGraphicsBuffer.unk854_2 == 0) {
            func_08003FB8();
            scene[0]++;
        }
    } else if (state > 1) {
        if ((state == 2) && ((gCurrentKeys & 0xF) != 0xF)) {
            return 1;
        }
    } else if (state == 0) {
        func_08006C40(0x14, 0);
        func_08002090(0x14);
        scene[0]++;
    }
    func_08006F68();
    func_08006B00();
    func_080041B4();
    return 0;
}
