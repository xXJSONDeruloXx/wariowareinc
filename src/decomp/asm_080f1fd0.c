#include "global.h"
#include "sound.h"
u16 func_080F1FD0(struct MidiBus *bus, u8 key) {
    u8 index = key;
    if ((s8)key < 0) {
        u32 corrected = 0;
        if (index <= 0xBE)
            corrected = 0x7F;
        index = corrected;
    }
    return bus->tuningTable[index];
}
