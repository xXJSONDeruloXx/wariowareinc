#include "global.h"
#include "scenes.h"
#include "src/lib_sprite.h"
extern void scene_set_current_thread(s32);
struct SceneSpriteState { u32 reserved[23]; s16 spriteId; };
void func_080B4240(struct SpriteHandler *handler){ struct SceneSpriteState *scene; scene_set_current_thread(1); scene=(struct SceneSpriteState*)gCurrentSceneVariable; sprite_set_enable_updates(handler,scene->spriteId,1); }
