#include "IMIDIEventReceiver.h"
#include <cassert>
#include <stdexcept>
#include <string>

void IMIDIEventReceiver::OnSysEventLoaded(uint32_t deltaTime, SysexEvent& sysEvent)
{
    if (sysEvent.type == ESysEvent::SONG_SELECT)
    {
        assert(sysEvent.messageLength == 1);
        uint8_t songNumber = *(uint8_t*)sysEvent.message;
    }

    PMIDISysEvent midiSysEvent;
    midiSysEvent.deltaTime = deltaTime;
    OnSysEvent(midiSysEvent);
}

void IMIDIEventReceiver::OnMetaEventLoaded(uint32_t deltaTime, MetaEvent& metaEvent)
{
    // AddTimeToTrack(currentTrackIndex, deltaTime);

    switch (metaEvent.type)
    {
    case EMidiMeta::SET_TEMPO:
    {
        assert(metaEvent.length == 3);
        Tempo tempo;
        tempo.deltaTime = deltaTime;
        tempo.newTempo = (metaEvent.bytes[0] << 16) | (metaEvent.bytes[1] << 8) | metaEvent.bytes[2];
        OnTempo(tempo);
        break;
    }

    case EMidiMeta::TIME_SIGNATURE:
    {
        assert(metaEvent.length == 4);
        TimeSignature timeSignature;
        timeSignature.deltaTime = deltaTime;
        int nominator = metaEvent.bytes[0];
        int denominator = 2 << metaEvent.bytes[1];
        int clocks = metaEvent.bytes[2];
        int notes = metaEvent.bytes[3];
        OnTimeSignature(timeSignature);
        break;
    }

    case EMidiMeta::KEY_SIGNATURE:
    {
        assert(metaEvent.length == 2);
        int8_t sf = (int8_t)metaEvent.bytes[0]; // 0 = key of C, -1 = 1 flat, 1 = 1 sharp
        uint8_t mi = metaEvent.bytes[1]; // major or minor ?

        sf = std::byteswap(sf); // TODO : only if machine is little endian

        KeySignature keySignature;
        keySignature.deltaTime = deltaTime;
        keySignature.sf = sf;
        keySignature.mi = mi;
        OnKeySignature(keySignature);

        // OnKeySignature(sf, mi);
    }
    break;

    case EMidiMeta::MIDI_PORT:
    {
        MIDIPort midiPort;
        midiPort.deltaTime = deltaTime;
        OnMIDIPort(midiPort);
        break;
    }
    case EMidiMeta::END_OF_TRACK:
    {
        EndOfTrack endOfTrack;
        endOfTrack.deltaTime = deltaTime;
        OnEndOfTrack(endOfTrack);
        break;
    }

        // case EMidiMeta::SEQ_SPECIFIC:
        //     break;

    case EMidiMeta::TEXT:
    {
        Text text;
        text.deltaTime = deltaTime;
        text.text = (const char*)metaEvent.bytes;
        text.length = metaEvent.length;
        OnText(text);
        // OnText((const char*)metaEvent.bytes, metaEvent.length);
        break;
    }
    case EMidiMeta::COPYRIGHT:
    {
        Copyright copyright;
        copyright.deltaTime = deltaTime;
        copyright.text = (const char*)metaEvent.bytes;
        copyright.length = metaEvent.length;
        OnCopyright(copyright);
        // OnCopyright((const char*)metaEvent.bytes, metaEvent.length);
        break;
    }
    case EMidiMeta::TRACK_NAME:
    {
        TrackName trackName;
        trackName.deltaTime = deltaTime;
        trackName.text = (const char*)metaEvent.bytes;
        trackName.length = metaEvent.length;
        OnTrackName(trackName);
        // OnTrackName((const char*)metaEvent.bytes, metaEvent.length);
        break;
    }
    case EMidiMeta::INSTRUMENT_NAME:
    {
        InstrumentName instrumentName;
        instrumentName.deltaTime = deltaTime;
        instrumentName.text = (const char*)metaEvent.bytes;
        instrumentName.length = metaEvent.length;
        OnInstrumentName(instrumentName);
        // OnInstrumentName((const char*)metaEvent.bytes, metaEvent.length);
        break;
    }
    case EMidiMeta::LYRICS:
    {
        Lyrics lyrics;
        lyrics.deltaTime = deltaTime;
        lyrics.text = (const char*)metaEvent.bytes;
        lyrics.length = metaEvent.length;
        OnLyrics(lyrics);
        // OnLyric((const char*)metaEvent.bytes, metaEvent.length);
        break;
    }
    case EMidiMeta::MARKER:
    {
        Marker marker;
        marker.deltaTime = deltaTime;
        marker.text = (const char*)metaEvent.bytes;
        marker.length = metaEvent.length;
        OnMarker(marker);
        // OnMarker((const char*)metaEvent.bytes, metaEvent.length);
        break;
    }
    case EMidiMeta::CUE_POINT:
    {
        CuePoint cuePoint;
        cuePoint.deltaTime = deltaTime;
        cuePoint.text = (const char*)metaEvent.bytes;
        cuePoint.length = metaEvent.length;
        OnCuePoint(cuePoint);
        // OnCuePoint((const char*)metaEvent.bytes, metaEvent.length);
        break;
    }

    default:
        // Deltatime should be used
        throw std::runtime_error(std::string("OnMetaEventLoaded/") + std::to_string((int)metaEvent.type) );
        break;
    }
}

void assertControlChange2(EControlChange cc, int param)
{
    assert(param >= 0);

    uint8_t ccValue = (int)cc;

    if (ccValue > 127)
    {
        throw std::runtime_error("Invalid Control Change Value : " + std::to_string(ccValue) + " (Higher than 127)");
    }

    if (ccValue <= 119)
    {
        if (param > 127)
            throw std::runtime_error("invalid param : ccValue <= 119 && param > 127");
        // assert(param <= 127);
    }
    // From 120 to 127 : channel modes 
    else if (ccValue == 120)
    {
        // Mute all sounds
    }
    else if (ccValue == 121)
    {
        // Reset All Controllers
    }
    else if (ccValue == 121)
    {
        // Local on/off Switch
    }
    else if (ccValue == 122)
    {
        if (param != 0 && param != 127)
            throw std::runtime_error("invalid param : ccValue == 122 && (param != 0 && param != 127)");
        // assert(param == 0 || param == 127);
    }
    else if (ccValue == 123)
    {
        // All Notes Off
    }
    else if (ccValue == 124)
    {
        // Omni Mode Off
    }
    else if (ccValue == 125)
    {
        //Omni Mode On
    }
    else if (ccValue == 126)
    {
        // Mono Mode (Monophonic)
    }
    else if (ccValue == 127)
    {
        // Poly Mode (Polyphonic)
    }
    else
    {
        if (param != 0)
        {
            // std::cout << "ERROR : " << (int) ccValue << std::endl;
            throw std::runtime_error("invalid param : ccValue is default, param != 0 : param == " + std::to_string(param) + " / ccValue == " + std::to_string(ccValue));
        }
        // assert(param == 0);
    }
}

void IMIDIEventReceiver::OnChannelEventLoaded(uint32_t deltaTime, ChannelEvent& channelEvent, bool isOpti)
{
    // AddTimeToTrack(currentTrackIndex, deltaTime);

    switch (channelEvent.message)
    {
        case ENoteEvent::NOTE_ON:
        {
            // parser.AddDebugLog(parser.debugBufferPtr->Data() - 2, "NoteOn:Key");
            // parser.AddDebugLog(parser.debugBufferPtr->Data() - 1, "NoteOn:Velocity");

            NoteOn noteOn;
            noteOn.deltaTime = deltaTime;
            noteOn.channel = channelEvent.channel;
            noteOn.key = channelEvent.param1;
            noteOn.velocity = channelEvent.param2;
            OnNoteOn(noteOn);

            // OnNoteOn(channelEvent.channel, channelEvent.param1, channelEvent.param2);
            break;
        }

        case ENoteEvent::NOTE_OFF:
        {
            // parser.AddDebugLog(parser.debugBufferPtr->Data() - 1, "NoteOff:Key");

            NoteOff noteOff;
            noteOff.deltaTime = deltaTime;
            noteOff.channel = channelEvent.channel;
            noteOff.key = channelEvent.param1;
            OnNoteOff(noteOff);

            // OnNoteOff(channelEvent.channel, channelEvent.param1);
            break;
        }

        case ENoteEvent::PGM_CHANGE:
        {
            // parser.AddDebugLog(parser.debugBufferPtr->Data() - 1, "ProgramChange:ID");

            ProgramChange programChange;
            programChange.deltaTime = deltaTime;
            programChange.channel = channelEvent.channel;
            programChange.newProgram = channelEvent.param1;
            OnProgramChange(programChange);


            // OnProgramChange(channelEvent.channel, channelEvent.param1);
            break;
        }

        case ENoteEvent::CONTROL_CHANGE:
        {
            // parser.AddDebugLog(parser.debugBufferPtr->Data() - 2, "ControlChange:Setting");
            // parser.AddDebugLog(parser.debugBufferPtr->Data() - 1, "ControlChange:Value");
            ControlChange controlChange;
            controlChange.deltaTime = deltaTime;
            controlChange.channel = channelEvent.channel;
            controlChange.ctrl = (EControlChange) channelEvent.param1;
            controlChange.value = channelEvent.param2;
            assertControlChange2((EControlChange) channelEvent.param1, channelEvent.param2);
            OnControlChange(controlChange);

            // OnControlChange(channelEvent.channel, (EControlChange) channelEvent.param1, channelEvent.param2);
            break;
        }

        case ENoteEvent::PITCH_BEND:
        {
            PitchBend pitchBend;
            pitchBend.deltaTime = deltaTime;
            pitchBend.channel = channelEvent.channel;
            pitchBend.value = channelEvent.param1;
            OnPitchBend(pitchBend);

            // OnPitchBend(channelEvent.channel, channelEvent.param1);
            break;
        }

        case ENoteEvent::NOTE_AFTER_TOUCH:
        {
            NoteAfterTouch noteAfterTouch;
            noteAfterTouch.deltaTime = deltaTime;
            OnNoteAfterTouch(noteAfterTouch);
            break;
        }

        case ENoteEvent::CHANNEL_AFTER_TOUCH:
        {
            ChannelAfterTouch channelAfterTouch;
            channelAfterTouch.deltaTime = deltaTime;
            OnChannelAfterTouch(channelAfterTouch);
            break;
        }

        default:
            throw std::runtime_error(std::string("OnChannelEventLoaded/") + std::to_string((int)channelEvent.message) + " / Opti : " + std::to_string(isOpti));
            break;
    }
}
