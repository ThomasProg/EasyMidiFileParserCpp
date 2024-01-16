#ifndef _MIDI_EXPORT_H_
#define _MIDI_EXPORT_H_

#include "Macros.h"
#include <cstddef>

struct BufferWriter;

// Returns the size of the save if buffer is null
MIDIPARSEREXPORT size_t SaveFileHeaderToBuffer(BufferWriter* buffer);
MIDIPARSEREXPORT size_t SaveFileHeaderDataToBuffer(BufferWriter* buffer, const struct FileHeaderData& fileHeaderData);
MIDIPARSEREXPORT size_t SaveTrackHeaderToBuffer(BufferWriter* buffer, const struct TrackHeader& trackHeaderData);
MIDIPARSEREXPORT size_t SaveDeltaTime(BufferWriter* buffer, uint32_t& deltaTime);
MIDIPARSEREXPORT size_t SaveMessageStatus(BufferWriter* buffer, const uint8_t& messageStatus);

MIDIPARSEREXPORT void SaveChannelEvent(BufferWriter& buffer, const struct ChannelEvent& channelEvent);
MIDIPARSEREXPORT void SaveNoteOn(BufferWriter& buffer, uint8_t channel, uint8_t key, uint8_t velocity);

MIDIPARSEREXPORT void SaveMetaEvent(BufferWriter& buffer);
MIDIPARSEREXPORT void SaveEndOfTrack(BufferWriter& buffer);

uint8_t VarToVarLen(uint32_t value, uint8_t* bytes);




#endif 