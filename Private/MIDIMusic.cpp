#include "MIDIMusic.h"


template<typename T>
void CopyEvent(MIDIMusicFiller& filler, const T& event)
{
    if (filler.music)
    {
        filler.music->tracks[filler.currentTrackIndex].midiEvents.emplace_back(new T(event));
    }
}

void MIDIMusicFiller::OnNoteOn(const NoteOn& noteOn)
{
    Super::OnNoteOn(noteOn);
    // NoteOn* noteOn = new NoteOn();
    // noteOn->start = timePerTrack[currentTrackIndex];
    // noteOn->channel = channel;
    // noteOn->key = key;
    // noteOn->velocity = velocity;

    CopyEvent(*this, noteOn);
}
void MIDIMusicFiller::OnNoteOff(const NoteOff& noteOff)
{
    Super::OnNoteOff(noteOff);
    CopyEvent(*this, noteOff);
}
void MIDIMusicFiller::OnProgramChange(const ProgramChange& programChange)
{
    Super::OnProgramChange(programChange);
    CopyEvent(*this, programChange);
}
void MIDIMusicFiller::OnControlChange(const ControlChange& controlChange)
{
    Super::OnControlChange(controlChange);
    CopyEvent(*this, controlChange);
}
void MIDIMusicFiller::OnPitchBend(const PitchBend& pitchBend)
{
    Super::OnPitchBend(pitchBend);
    CopyEvent(*this, pitchBend);
}

void MIDIMusicFiller::OnNoteAfterTouch(const NoteAfterTouch& noteAfterTouch)
{
    Super::OnNoteAfterTouch(noteAfterTouch);
    CopyEvent(*this, noteAfterTouch);
}
void MIDIMusicFiller::OnChannelAfterTouch(const ChannelAfterTouch& channelAfterTouch)
{
    Super::OnChannelAfterTouch(channelAfterTouch);
    CopyEvent(*this, channelAfterTouch);
}


void MIDIMusicFiller::OnKeySignature(const KeySignature& keySignature)
{
    Super::OnKeySignature(keySignature);
    CopyEvent(*this, keySignature);
}
void MIDIMusicFiller::OnTimeSignature(const TimeSignature& timeSignature)
{
    Super::OnTimeSignature(timeSignature);
    CopyEvent(*this, timeSignature);
}
void MIDIMusicFiller::OnMIDIPort(const MIDIPort& midiPort)
{
    Super::OnMIDIPort(midiPort);
    CopyEvent(*this, midiPort);
}
void MIDIMusicFiller::OnEndOfTrack(const EndOfTrack& endOfTrack)
{
    Super::OnEndOfTrack(endOfTrack);
    CopyEvent(*this, endOfTrack);
}
void MIDIMusicFiller::OnTempo(const Tempo& tempo)
{
    Super::OnTempo(tempo);
    CopyEvent(*this, tempo);
}
void MIDIMusicFiller::OnText(const Text& text)
{
    Super::OnText(text);
    CopyEvent(*this, text);
}
void MIDIMusicFiller::OnCopyright(const Copyright& copyright)
{
    Super::OnCopyright(copyright);
    CopyEvent(*this, copyright);
}
void MIDIMusicFiller::OnTrackName(const TrackName& trackName)
{
    Super::OnTrackName(trackName);
    CopyEvent(*this, trackName);
}
void MIDIMusicFiller::OnInstrumentName(const InstrumentName& instrumentName)
{
    Super::OnInstrumentName(instrumentName);
    CopyEvent(*this, instrumentName);
}
void MIDIMusicFiller::OnLyrics(const Lyrics& lyrics)
{
    Super::OnLyrics(lyrics);
    CopyEvent(*this, lyrics);
}
void MIDIMusicFiller::OnMarker(const Marker& marker)
{
    Super::OnMarker(marker);
    CopyEvent(*this, marker);
}
void MIDIMusicFiller::OnCuePoint(const CuePoint& cuePoint)
{
    Super::OnCuePoint(cuePoint);
    CopyEvent(*this, cuePoint);
}


void MIDIMusicFiller::OnSysEvent(const PMIDISysEvent& sysEvent)
{
    Super::OnSysEvent(sysEvent);
    CopyEvent(*this, sysEvent);
}

void MIDIMusicFiller::OnUnsupportedEvent(const UnsupportedEvent& unsupportedEvent)
{
    Super::OnUnsupportedEvent(unsupportedEvent);
    CopyEvent(*this, unsupportedEvent);
}

void MIDIMusicFiller::OnEvent(const PMIDIEvent& event)
{

}