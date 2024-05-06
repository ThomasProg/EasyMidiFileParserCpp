#pragma once

#include "Macros.h"

extern "C"
{
    MIDIPARSEREXPORT bool MIDIMusic_LoadFromFile(class MIDIMusic* music, const char* filename);
}
