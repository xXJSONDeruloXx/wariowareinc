#include "global.h"

struct Func08002090SoundPlayerEntry {
    void *soundPlayer;
    u32 unused;
    u16 totalTracks;
    u16 priorityEnabled;
};

extern u8 sound_player_count;
extern struct Func08002090SoundPlayerEntry sound_player_table[];
extern void func_080F30E0(u32 arg0, u32 arg1);

void func_08002090(u32 arg0) {
    u32 i;
    u32 value;
    u8 *count;
    struct Func08002090SoundPlayerEntry *table;

    arg0 <<= 16;
    i = 0;
    if (i < sound_player_count) {
        arg0 >>= 20;
        value = arg0 << 16;
        table = sound_player_table;
        count = &sound_player_count;
        do {
            func_080F30E0((u32)table->soundPlayer, value >> 16);
            table++;
            i++;
        } while (i < *count);
    }
}
