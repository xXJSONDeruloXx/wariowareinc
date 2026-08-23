#if __INCLUDE_LEVEL__ > 0
#include "global.h"
#include "scenes.h"

extern u32 get_current_language(void);
extern u32 D_083AB320[];
extern void func_08015A88(void);

struct Func08014374SceneTable {
    u32 entries[1];
};

struct Func08014374LanguageEntry {
    struct Func08014374SceneTable *scene_table;
};

struct Func08014374SceneData {
    u8 padding[0xFD];
    u8 scene_index;
};

typedef void (*Func08014374Load)(u32);

void func_08014374(void) {
    u32 language;
    struct Func08014374LanguageEntry *language_table;
    struct Func08014374LanguageEntry *language_entry;
    struct Func08014374SceneData *scene;
    u8 scene_index;
    struct Func08014374SceneTable *scene_table;
    u32 offset;
    u32 value;

    language = get_current_language();
    language_table = (struct Func08014374LanguageEntry *)D_083AB320;
    language_entry = &language_table[language];
    scene = (struct Func08014374SceneData *)gCurrentSceneData;
    scene_index = scene->scene_index;
    scene_table = language_entry->scene_table;
    offset = scene_index << 2;
    offset += (u32)scene_table;
    value = *(u32 *)offset;
    ((Func08014374Load)func_08015A88)(value);
}
#endif
