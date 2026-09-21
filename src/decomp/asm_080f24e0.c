#include "global.h"
#include "sound.h"
void func_080F24E0(struct MidiBus *bus, s32 channel, u16 value) {
    u16 bank = bus->midiChannel[channel].bankSelect;
    if (value & 0x8000)
        bank = (bank & 0x3F80) | (value << 7);
    else
        bank = (bank & 0x7F) | value;
    bus->midiChannel[channel].bankSelect = bank & 0x3FFF;
}
