#include "global.h"
#include "types.h"

void func_0804BF4C(void);
void func_0804C058(void);

void func_0804BE68(void) {
    if (((u32 *)gCurrentSceneVariable)[24] != 1) {
        func_0804BF4C();
        func_0804C058();
    }
}
