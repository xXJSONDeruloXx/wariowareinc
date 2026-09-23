#include "global.h"
#include "scenes.h"
#include "src/scenes/gameplay.h"
extern void func_0800A200(u32);
extern void func_08009EE0_stub(s32);
struct Func08024628SceneVariable { u32 value; u8 active : 1; u8 rest : 7; };
void func_08024628(void) {
    struct Func08024628SceneVariable *variable;
    struct GameplayData *gameplay;
    func_0800A200(0);
    func_08009EE0_stub(0);
    variable = (struct Func08024628SceneVariable *)gCurrentSceneVariable;
    variable->active = 0;
    gameplay = (struct GameplayData *)gCurrentSceneData;
    gameplay->currentLives = 1;
}
