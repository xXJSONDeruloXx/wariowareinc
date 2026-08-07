#include "global.h"
#include "scenes.h"

extern void func_08012058(void);
extern void func_08012700(u32);
extern void func_0801646C(void);
extern void func_080164CC(void);
extern void func_08016520(void);

void func_080165D4(void) {
    u8 *scene;
    u8 state;
    u32 mask;
    u32 index;

    scene = (u8 *)gCurrentSceneData;
    if ((scene[0xDF] << 0x1F) != 0) {
        state = *(u8 *)(scene + 0x1B8);
        switch (state) {
        case 0:
            func_0801646C();
            break;
        case 1:
            func_080164CC();
            break;
        case 2:
            func_08016520();
            break;
        }
        scene = (u8 *)gCurrentSceneData;
        if ((scene[0xDF] << 0x1F) == 0) {
            mask = *(u32 *)(scene + 0x1B4);
            index = 0;
            if ((mask & 1) == 0) {
                do {
                    index += 1;
                    if (index > 0x1B) {
                        break;
                    }
                } while (((mask >> index) & 1) == 0);
            }
            if ((index - 2) <= 1) {
                index = 5;
            }
            if ((index == 7) || (index == 4)) {
                index = 6;
            }
            func_08012700(index);
            func_08012058();
        }
    }
}
