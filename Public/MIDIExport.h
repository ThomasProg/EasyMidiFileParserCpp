#ifndef _MIDI_EXPORT_H_
#define _MIDI_EXPORT_H_

#include "Macros.h"
#include <cstdint>

struct BufferWriter;

// Returns the size of the save if buffer is null
MIDIPARSEREXPORT std::size_t SaveFileHeaderToBuffer(BufferWriter* buffer);
MIDIPARSEREXPORT std::size_t SaveFileHeaderDataToBuffer(BufferWriter* buffer, const struct FileHeaderData& fileHeaderData);
MIDIPARSEREXPORT std::size_t SaveTrackHeaderToBuffer(BufferWriter* buffer, const struct TrackHeader& trackHeaderData);
MIDIPARSEREXPORT std::size_t SaveDeltaTime(BufferWriter* buffer, std::uint32_t& deltaTime);
MIDIPARSEREXPORT std::size_t SaveMessageStatus(BufferWriter* buffer, const std::uint8_t& messageStatus);

MIDIPARSEREXPORT void SaveChannelEvent(BufferWriter& buffer, const struct ChannelEvent& channelEvent);
MIDIPARSEREXPORT void SaveNoteOn(BufferWriter& buffer, std::uint8_t channel, std::uint8_t key, std::uint8_t velocity);

MIDIPARSEREXPORT void SaveMetaEvent(BufferWriter& buffer);
MIDIPARSEREXPORT void SaveEndOfTrack(BufferWriter& buffer);

std::uint8_t VarToVarLen(std::uint32_t value, std::uint8_t* bytes);




#endif 