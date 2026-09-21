#include "global.h"
#include "sound.h"
void func_080F2598(struct MidiBus *bus, s32 channel, u8 filterEQ) {
    bus->midiChannel[channel].filterEQ = filterEQ;
}
