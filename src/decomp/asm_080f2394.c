#include "global.h"
#include "sound.h"
extern void func_080F23F8(struct MidiBus *, s32);
void func_080F2394(struct MidiBus *bus, s32 channel, u8 panning) {
    bus->midiChannel[channel].panning = panning;
    func_080F23F8(bus, channel);
}
