#ifndef _BUFFER_WRITER_H_
#define _BUFFER_WRITER_H_

#include <cstdint>
#include <stdexcept>
#include <concepts>
#include <cstring> // memcpy
#include "EndianHelpers.h"
#include "Macros.h"

struct MIDIPARSEREXPORT BufferWriter
{
    uint8_t* bufferStart = nullptr;
    size_t bufferSize = 0;
    size_t offset = 0;

    #ifdef MAKE_DLL

    void CheckSize()
    {
        if (offset > bufferSize)
        {
            throw std::runtime_error("offset > buffer.size");
        }
    }

    uint8_t* Consume(size_t size)
    {
        uint8_t* bufferReturned = bufferStart + offset;
        offset += size;
        CheckSize();
        return bufferReturned;
    }

    void GoBack(size_t size)
    {
        offset -= size;
    }

    template<typename T>
    void SaveRaw(const T& data)
    {
        memcpy(bufferStart + offset, &data, sizeof(data));
        offset += sizeof(data);

        CheckSize();
    }


    template<typename T>
    void Save(const T& data)
    {
        SaveRaw<T>(data);
    }

    void Save(const char* str, uint32_t strLength)
    {
        const uint32_t byteLength = strLength * sizeof(*str);
        memcpy(bufferStart + offset, str, byteLength);
        offset += byteLength;

        CheckSize();
    }

    void Save(const uint8_t* array, uint32_t arrayLength)
    {
        Save((const char*) array, arrayLength);
    }

    template<typename T>
    requires std::integral<T>
    void Save(const T& data)
    {
        memcpy(bufferStart + offset, &data, sizeof(data));
        NativeToBigEndian<T>(* (T*) (bufferStart + offset));
        offset += sizeof(data);
        
        CheckSize();
    }

    #endif
};

#endif