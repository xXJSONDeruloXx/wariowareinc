#include "global.h"
#include "sound.h"
void func_080F253C(struct MidiBus *bus, s32 channel, u8 disabled) {
    bus->midiChannel[channel].disabled = disabled;
}
