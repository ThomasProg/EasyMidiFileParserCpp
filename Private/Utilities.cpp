#include "Utilities.h"

#include "MIDIParserBase.h"
#include "MIDIMusic.h"
#include "MIDIMusicFiller.h"

bool MIDIMusic_LoadFromFile(class MIDIMusic* music, const char* filename)
{
    MIDIMusicFiller filler;
    filler.music = music;

    MIDIParserBase parser;
    parser.observer = &filler;
    try
    {
        parser.LoadFromFile(filename);
        return true;
    }
    catch(...)
    {
        return false;
    }
}