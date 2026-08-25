#include "global.h"

typedef struct Func08007E68State Func08007E68State;
typedef void (*Func08007E68Decompress)(Func08007E68State *);

struct Func08007E68State {
    u8 pad0[0x1C];
    u32 field1C;
    u32 field20;
};

extern u32 gfx_decompress_rom;
extern u8 D_03000E38[];

void func_08007E68(Func08007E68State *state) {
    Func08007E68Decompress decompress;

    if (state == 0) {
        state = (Func08007E68State *)D_03000E38;
    }
    state->field1C = state->field20;
    decompress = (Func08007E68Decompress)&gfx_decompress_rom;
    decompress(state);
}
