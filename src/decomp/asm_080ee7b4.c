#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/lib_sprite.h"

extern void sprite_handler_reset(struct SpriteHandler *);

/* One 0x20-byte OAM clear chunk written by sprite_handler_create's fill loop. */
struct OamClearChunk {
    u32 a;
    u32 b;
    u32 c;
    u32 d;
    u32 e;
    u32 f;
    u32 g;
    u32 h;
};

/* Named overlay over struct SpriteHandler's error window: the error-type
   storage at handler+0x24 is a bitfield unit in the project record, so the
   original byte-level clear is modeled through these named members anchored
   at handler->unk20 instead. */
struct SpriteHandlerErrView {
    u16 unk20;
    u8 errorType;
};

void sprite_handler_create(struct SpriteHandler *handler, s32 objAmountArg, u32 *oamBuffer, s32 spriteAmountArg, struct Sprite *sprites) {
    u32 i;
    u32 j;
    u32 bound;
    struct OamClearChunk *chunk;
    u32 *word;
    u32 mask;
    u8 *flags;
    u16 objAmount;
    u16 spriteAmount;

    objAmount = (u16)objAmountArg;
    spriteAmount = (u16)spriteAmountArg;
    handler->objAmount = objAmount;
    handler->oamBuffer = oamBuffer;
    chunk = (struct OamClearChunk *)oamBuffer;
    i = 0;
    bound = objAmount >> 3;
    while (i < bound) {
        chunk->a = chunk->b = chunk->c = chunk->d = chunk->e = chunk->f = chunk->g = chunk->h = 0x22222222;
        chunk++;
        i++;
    }
    i = 0;
    j = objAmount & 7;
    word = (u32 *)chunk;
    flags = &((struct SpriteHandlerErrView *)&handler->unk20)->errorType;
    while (i < j) {
        *word++ = 0x22222222;
        i++;
    }
    handler->spriteAmount = spriteAmount;
    handler->sprites = sprites;
    handler->paused = 0;
    handler->xPos = 0;
    handler->yPos = 0;
    handler->totalCycles = 0;
    handler->memID = 0;
    *(u32 *)&handler->unk1E = 0xFF;
    bound = *flags;
    mask = -0x10;
    mask = mask & bound;
    *flags = mask;
    sprite_handler_reset(handler);
}
#endif
