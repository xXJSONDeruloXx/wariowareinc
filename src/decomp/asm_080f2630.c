#include "global.h"
#include "sound.h"

void func_080F2630(struct MidiBus *bus, s32 channel, u8 priority) {
    bus->midiChannel[channel].priority = (priority + bus->priority) & 0xFF;
}
