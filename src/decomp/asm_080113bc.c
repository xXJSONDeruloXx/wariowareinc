#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/scenes/main_menu.h"

extern void func_080121D0(void);
extern void func_0801312C(void);
extern void func_080148EC(void);
extern void func_08014C9C(void);
extern void func_08014FF4(void);
extern void func_08011864(u32);
extern void func_08015C7C(u8);

void func_080113BC(void) {
    func_080121D0();
    func_0801312C();
    func_080148EC();
    func_08014C9C();
    func_08014FF4();
    func_08011864(D_03006518.unk2);
    func_08015C7C(D_03006518.unk2);
}
#endif
