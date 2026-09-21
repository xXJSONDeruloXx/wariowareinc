#include "global.h"
#include "sound.h"
extern void func_080F2394(struct MidiBus *, s32, u32);
void func_080F2608(struct MidiBus *bus, s32 channel, u32 phaseStereo) {
    bus->midiChannel[channel].phaseStereo = phaseStereo;
    func_080F2394(bus, channel, bus->midiChannel[channel].panning);
}
