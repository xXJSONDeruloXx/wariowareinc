#include "global.h"
#include "types.h"

void func_080ED380(void);
void func_080EC960(void);

void func_080ECA78(void) {
    ((u8 *)gCurrentSceneVariable)[0xF4] = 6;
    func_080ED380();
    func_080EC960();
}
