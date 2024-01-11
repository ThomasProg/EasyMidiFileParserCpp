#include "Utilities.h"

#include "MIDIParserBase.h"
#include "MIDIMusic.h"
#include "MIDIMusicFiller.h"

void MIDIMusic_LoadFromFile(class MIDIMusic* music, const char* filename)
{
    MIDIMusicFiller filler;
    filler.music = music;

    MIDIParserBase parser;
    parser.observer = &filler;
    parser.LoadFromFile(filename);
}