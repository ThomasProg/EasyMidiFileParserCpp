#include "MIDIExport.h"
#include "Buffers/BufferWriter.h"
#include "MIDIHelpers.h"

// uint8_t bytes[4];
// returns length
uint8_t VarToVarLen(uint32_t value, uint8_t* bytes)
{
    uint32_t buffer;
    buffer = value & 0x7F;

    while ( (value >>= 7) )
    {
        buffer <<= 8;
        buffer |= ((value & 0x7F) | 0x80);
    }

    uint8_t index = 0;
    while (true)
    {   
        bytes[index] = *(uint8_t*)&buffer;
        index++;
        // putc(buffer, outfile);
        if (buffer & 0x80)
            buffer >>= 8;
        else
            break;
    }
    return index;
}


uint8_t WriteVarLen(uint32_t value, BufferWriter& bufferWriter)
{
    uint32_t buffer;
    buffer = value & 0x7F;

    while ( (value >>= 7) )
    {
        buffer <<= 8;
        buffer |= ((value & 0x7F) | 0x80);
    }

    uint8_t index = 0;
    while (true)
    {   
        bufferWriter.Save(*(uint8_t*)&buffer);
        index++;
        // putc(buffer, outfile);
        if (buffer & 0x80)
            buffer >>= 8;
        else
            break;
    }
    return index;
}


// Returns the size of the save if buffer is null
size_t SaveFileHeaderToBuffer(BufferWriter* buffer)
{
    constexpr char headerName[] = {'M', 'T', 'h', 'd'};
    constexpr uint32_t headerDataSize = 6;
    
    if (buffer != nullptr)
    {
        buffer->Save(headerName);
        buffer->Save(headerDataSize);
    }

    return sizeof(headerName) + sizeof(headerDataSize);
}

// Returns the size of the save if buffer is null
size_t SaveFileHeaderDataToBuffer(BufferWriter* buffer, const FileHeaderData& fileHeaderData)
{
    constexpr size_t size = 3 * sizeof(uint16_t);
    if (buffer)
    {
        buffer->Save(fileHeaderData.format);
        buffer->Save(fileHeaderData.nbTracks);

        if (fileHeaderData.isTicksPerFrame)
        {
            buffer->SaveRaw(-fileHeaderData.delta.perFrame.SMPTEFormat);
            buffer->SaveRaw(fileHeaderData.delta.perFrame.ticksPerFrame);
        }
        else 
        {
            // TODO : fix ? (see parsing)
            // fileHeaderData.delta.ticksPerQuarterNote = ((unsigned)division[0] << 8) | ((unsigned)division[1] & 0xff);
            buffer->Save(fileHeaderData.delta.ticksPerQuarterNote);
        }
    }
    return size;
}

// Returns the size of the save if buffer is null
size_t SaveTrackHeaderToBuffer(BufferWriter* buffer, const TrackHeader& trackHeaderData)
{
    if (buffer)
    {
        buffer->SaveRaw(trackHeaderData.type);
        buffer->Save(trackHeaderData.length);
    }
    return sizeof(trackHeaderData.type) + sizeof(trackHeaderData.length);
}

// TODO
size_t SaveDeltaTime(BufferWriter* buffer, uint32_t& deltaTime)
{
    if (buffer)
    {
        return WriteVarLen(deltaTime, *buffer);
    }
    return 0;
}

// TODO
size_t SaveMessageStatus(BufferWriter* buffer, const uint8_t& messageStatus)
{
    if (buffer)
    {
        buffer->SaveRaw(messageStatus);
    }
    return sizeof(messageStatus);
}

// // TODO
// size_t SaveSysEvent(BufferWriter* buffer, const SysexEvent& sysEvent)
// {
//     if (buffer)
//     {
//         buffer->SaveRaw(sysEvent.type);
//         WriteVarLen(sysEvent.messageLength, *buffer);
//         buffer->Save(sysEvent.message, sysEvent.messageLength);
//     }
    
//     return 0;//sizeof(messageStatus);
// }

void SaveChannelEvent(BufferWriter& buffer, const ChannelEvent& channelEvent)
{
    uint8_t channel = channelEvent.channel;
    NativeToBigEndian(channel);
    buffer.Save((uint8_t) channelEvent.message | channel);

    int dataLength = GetMIDIEventDataLength(channelEvent.message);

    
}


void SaveNoteOn(BufferWriter& buffer, uint8_t channel, uint8_t key, uint8_t velocity)
{
    NativeToBigEndian(channel);
    buffer.SaveRaw<uint8_t>((uint8_t) ENoteEvent::NOTE_ON | channel);

    // int dataLength = GetMIDIEventDataLength(ENoteEvent::NOTE_ON);
    buffer.Save(key);
    buffer.Save(velocity);
}

void SaveMetaEvent(BufferWriter& buffer, const MetaEvent& metaEvent)
{
    buffer.SaveRaw(uint8_t(0xff));
    buffer.SaveRaw(metaEvent.type);
    WriteVarLen(metaEvent.length, buffer);
    buffer.Save(metaEvent.bytes, metaEvent.length);
}

void SaveEndOfTrack(BufferWriter& buffer)
{
    MetaEvent metaEvent;
    metaEvent.type = EMidiMeta::END_OF_TRACK;
    metaEvent.length = 0;
    metaEvent.bytes = nullptr;
    SaveMetaEvent(buffer, metaEvent);

    // buffer.SaveRaw(uint8_t(0xff));
    // buffer.SaveRaw(metaEvent.type);
    // WriteVarLen(metaEvent.length, buffer);
    // buffer.Save(metaEvent.bytes, metaEvent.length);
}