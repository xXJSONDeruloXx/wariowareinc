#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "src/lib_sprite.h"

extern u32 get_current_mem_id(void);

void scene_set_current_thread(u32 arg0) {
    gBeatscriptScene.currentThread = arg0;
    sprite_handler_set_mem_id(gSpriteHandler, get_current_mem_id());
    gCurrentSceneVariable = &gBeatscriptScene.localVariables[arg0];
    gCurrentSceneSpritePool = gBeatscriptScene.threads[arg0].sprites;
}
#endif
