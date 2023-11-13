#ifndef _BUFFER_READER_H_
#define _BUFFER_READER_H_

#include <cstdint>
#include <stdexcept>

#ifdef MAKE_DLL // CPP
#define MIDIPARSEREXPORT __declspec(dllexport)
#else
#define MIDIPARSEREXPORT __declspec(dllimport)
#endif

struct MIDIPARSEREXPORT Buffer
{
    const uint8_t* bufferStart;
    size_t size;
};

class MIDIPARSEREXPORT BufferReader
{
    const Buffer& buffer;
    size_t offset = 0;

public:
    BufferReader(Buffer& newBuffer) : buffer(newBuffer) {}

    const uint8_t* Consume(size_t size)
    {
        const uint8_t* bufferReturned = buffer.bufferStart + offset;
        offset += size;
        if (offset > buffer.size)
        {
            throw std::runtime_error("offset > buffer.size");
        }
        return bufferReturned;
    }

    void GoBack(size_t size)
    {
        offset -= size;
    }

    const uint8_t* Data() const
    {
        return buffer.bufferStart + offset;
    }
};


#endif