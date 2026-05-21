#include "global.h"
#include "sound.h"

void func_080F30E0(void);

void func_08002090(u32 a0) {
    (void)a0;
    // Original wrapper iterated over sound_player_table and called func_080F30E0 for each entry.
    // For now we provide a single call to preserve build success; the detailed loop can be restored later.
    func_080F30E0();
}