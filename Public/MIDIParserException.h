#pragma once

#include <string>
#include <vector>
#include <format>

#include "MIDIParser.h"

class MIDIParserException
{
    const MIDIParser& parser;
    std::vector<uint8_t> bytes;
    const char* bytesPtr;
    std::string str;

    int past = -20;

private:
    std::string BytesToStr(const std::vector<uint8_t>& bytes) 
    {
        std::string s;
        for (int i = 0; i < bytes.size(); i++)
        {
            uint8_t* bytePtr = (uint8_t*) (bytesPtr - 50 + i); 
            const uint8_t& byte = bytes[i];
            if (parser.byteToDataStr.contains(bytePtr))
            {
                 const std::string& f = parser.byteToDataStr.find(bytePtr)->second;
                s += std::format("0x{:x} ({:b}) ({}) ({})\n", byte, byte, std::to_string((int) byte), f);
            }
            else 
            {
                s += std::format("0x{:x} ({:b}) ({})\n", byte, byte, std::to_string((int) byte));
                // s += std::to_string((int) byte) + "  ";
            }
        }
        return s;
    }

    std::vector<uint8_t> RetrieveBytes(const char* buffer)
    {
        std::vector<uint8_t> bytes;
        for (int i = -50; i < 5; i++)
        {
            bytes.push_back((uint8_t) *(buffer + i));
        }
        return bytes;
    }

public:

    MIDIParserException(const MIDIParser& newParser, const char* message, const char* newBuffer) 
        : parser(newParser), 
        bytes(RetrieveBytes(newBuffer)), 
         bytesPtr(newBuffer), 
        str(BytesToStr(bytes) + message) 
    {

    }
    MIDIParserException(const MIDIParser& newParser, const std::string& message, const char* newBuffer) 
        : parser(newParser), 
        bytes(RetrieveBytes(newBuffer)), 
         bytesPtr(newBuffer), 
        str(BytesToStr(bytes) + message) 
    {

    }
    MIDIParserException(const MIDIParser& newParser, std::string&& message, const char* newBuffer) 
        : parser(newParser), 
        bytes(RetrieveBytes(newBuffer)), 
         bytesPtr(newBuffer), 
        str(BytesToStr(bytes) + std::move(message)) 
    {

    }

    const char* what() const
    {
        return str.c_str();
    }
};