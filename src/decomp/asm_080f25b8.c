#include "global.h"
#include "sound.h"
void func_080F25B8(struct MidiBus *bus, s32 channel, u8 modType) {
    bus->midiChannel[channel].modType = modType;
}
