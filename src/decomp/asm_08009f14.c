#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "types.h"
#include "src/audio.h"

extern s32 func_0800A430(s32);
extern void update_beatscript_tempo(void);
extern void scene_update_music_pitch(void);
extern void set_soundplayer_volume(struct SoundPlayer *, u16);

u32 scene_change_music(struct SongHeader *music, u32 override) {
    register u32 r0 asm("r0") = (u32)music;
    register u32 r1 asm("r1");
    register u32 r2 asm("r2") = override;
    register u32 r4 asm("r4");
    register u32 r5 asm("r5") = (u32)music;

    r1 = (u32)&gBeatscriptScene;
    r1 = *(u32 *)(r1 + 4);
    if (r1 == 0) goto skip_stop;
    if (r2 == 0) goto skip_stop;
    r0 = r1;
    stop_soundplayer((struct SoundPlayer *)r0);
skip_stop:
    if (r5 != 0) goto play_music;
    r1 = (u32)&gBeatscriptScene;
    *(u32 *)(r1 + 4) = r5;
    goto done;

play_music:
    r0 = r5;
    r0 = (u32)play_sound((struct SongHeader *)r0);
    r4 = (u32)&gBeatscriptScene;
    *(u32 *)(r4 + 4) = r0;
    r0 = r5;
    r0 = (u32)func_0800A430((s32)r0);
    *(u16 *)(r4 + 8) = r0;
    update_beatscript_tempo();
    scene_update_music_pitch();
    r0 = *(u32 *)(r4 + 4);
    r1 = 0x1C58;
    r4 += r1;
    r1 = *(u16 *)r4;
    set_soundplayer_volume((struct SoundPlayer *)r0, r1);

done:
    return r0;
}
#endif
