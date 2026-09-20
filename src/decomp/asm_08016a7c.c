#include "global.h"
#include "scenes.h"
struct SceneBits { u8 pad[0x4A]; u8 bit0:1; u8 bit1:1; u8 rest:6; };
void func_08016A7C(u32 value){ ((struct SceneBits*)gCurrentSceneData)->bit1=value; }
