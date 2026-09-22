#include "global.h"
struct Scene855FC { u8 sceneState[0xC58]; s16 spriteId; };
struct SpriteHandler;
extern struct SpriteHandler *gSpriteHandler;
extern s32 sprite_set_visible(struct SpriteHandler *, s16, u16);
s32 func_080855FC(void) { return sprite_set_visible(gSpriteHandler, ((struct Scene855FC *)gCurrentSceneVariable)->spriteId, 0); }
