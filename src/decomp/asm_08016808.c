#include "global.h"
#include "graphics.h"
#include "scenes.h"
#include "src/beatscript.h"
#include "src/code_08000f10.h"

extern void func_08016A9C(void);
extern void func_08016B34(void);
extern void func_08016C60(void);
extern void func_08016798(void);
extern void func_080167D4(void);

void func_08016808(void) {
    u16 state;

    func_08016A9C();
    func_08016B34();
    func_08016C60();
    state = *(u16 *)((u8 *)gCurrentSceneData + 0x3A);
    switch (state) {
    case 0:
        func_08016798();
        break;
    case 1:
        func_080167D4();
        break;
    }
    func_08009EE4((gCurrentKeys >> 8) & 1);
}
