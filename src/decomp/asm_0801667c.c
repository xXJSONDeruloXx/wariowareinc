#include "global.h"

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u8 unk4;
    u8 unk5;
    u8 unk6;
} UnkStruct_03006518;

#define gUnk03006518 (*(UnkStruct_03006518 *)0x03006518)

s32 func_0801667C(void) {
    return gUnk03006518.unk6;
}
