#include "global.h"
#include "types.h"

struct SoundPlayer;
extern void func_080DF2C4(struct SoundPlayer *, u16 *);

struct Func08075EA4State {
    u8 padding[0x7C];
    struct SoundPlayer *player;
    u16 pitch;
    u8 enabled;
};

void func_08075EA4(void)
{
    struct Func08075EA4State *state;

    state = (struct Func08075EA4State *)gCurrentSceneVariable;
    if (state->enabled != 0) {
        func_080DF2C4(state->player, &state->pitch);
    }
}
