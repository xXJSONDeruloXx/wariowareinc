#include "global.h"
#include "graphics.h"
#include "src/beatscript.h"
#include "src/code_08000f10.h"

extern void func_080069F4(void);
extern void func_08006A04(void);
extern void func_08006B90(u32);
extern void func_08006B68(void);
extern void func_08006F28(void);
extern void func_08003E64(void);
extern void func_08016D7C(void);

void func_08016CBC(u32 subScene) {
    const struct SubScene *subScenes[4];

    func_080069F4();
    func_08006A04();
    func_08006B90(0);
    func_08006B68();
    func_08006F28();
    func_08003E64();
    func_08000F74(func_08016D7C);
    subScenes[0] = (const struct SubScene *)subScene;
    subScenes[1] = NULL;
    start_beatscript_scene(0);
    set_beatscript_subscenes(subScenes);
}
