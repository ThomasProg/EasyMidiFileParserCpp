# Easy Midi File Parser Cpp

This parser works on .midi files, following the Standard MIDI Files 1.0 specification (SMF 1.0).

## Motivation

Some MIDI parser implementations include a lot of things to treat runtime MIDI and other midi messages.

Some implementations exist in C.

But it wasn't easy to find a **simple** and **good** C++ parser that was really understandable.

If you want to understand how the MIDI file format is made, I encourage you to look at the code.\
There are no global variables, and even tho the parsing functions are in a class, there are no variable members.\
It means all the dependencies are given directly through the functions, without any magic.

If you want to access the parsing, just inherit from the MIDIParser class and override the callbacks.

An example is given in the https://github.com/ThomasProg/ModularMusicGenerationCore project, where fluidsynth is used to play the notes after the parsing is made.

My personal motivation for this parser is that I want to be able to convert this format into another one, easier to deal with. For these reasons, the library might evolve to add abstractions. However, if it does, MIDIParser won't be changed, and will only be inherited.

## Dependencies 

None, except the standard library.

C++ 23 is used, but only in the .cpp.\
The .h only includes <cstdint> (types with a specific size), so it should make it fast to compile.\
It also means you should be able to link it to other versions of C++ without any conflict.

## Support 

- [X] [Standard MIDI Files 1.0 specification (SMF)](https://www.midi.org/specifications/file-format-specifications/standard-midi-files)
- [X] Format 0 Supported
- [X] Format 1 Supported
- [ ] Format 2 Supported (uncommon)
- [X] Little Endian Support (most common)
- [ ] Big Endian Support (old machines)
- [ ] Mixed Endian Support (weird machines)
- [x] Loading
- [ ] Conversion to an easier format

## Other notes

Merge requests are welcome, even if it's just to add comments and clarification!



