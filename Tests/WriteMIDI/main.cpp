#include <filesystem>
#include <iostream>
#include <fstream>
#include "MIDIParserException.h"
#include "LoggingMIDIParser.h"
#include "MIDIExport.h"
#include "Buffers/BufferWriter.h"

void displayError(const std::string& s)
{
    std::cout << "\033[1;31m" << s << "\033[0m\n" << std::endl;    
}

void displaySuccess(const std::string& s)
{
    std::cout << "\033[1;32m" << s << "\033[0m\n" << std::endl;    
}

void WriteMIDI()
{   
    // std::cout << "Please enter path : " << std::endl;

    // std::string path;
    // std::cin >> path;

    uint32_t defaultTempo = 500000; // 120 bpm by default 

    BufferWriter writer;
    writer.bufferSize = 10000; // TODO
    writer.bufferStart = (uint8_t*) malloc(writer.bufferSize); 

    SaveFileHeaderToBuffer(&writer);

    FileHeaderData fileHeaderData;
    fileHeaderData.format = 0;
    fileHeaderData.nbTracks = 1;
    fileHeaderData.isTicksPerFrame = false;
    fileHeaderData.delta.ticksPerQuarterNote = 480;

    SaveFileHeaderDataToBuffer(&writer, fileHeaderData);
    



    BufferWriter notePlayBuffer;
    notePlayBuffer.bufferSize = 5*(4 + sizeof(uint8_t)*3) + (4 + 5); // TODO
    notePlayBuffer.bufferStart = (uint8_t*) malloc(notePlayBuffer.bufferSize); 

    // Track data
    {
        uint32_t deltaTime = 2 * 1000 * fileHeaderData.delta.ticksPerQuarterNote / defaultTempo *1000/*why?*/;
        size_t t = SaveDeltaTime(&notePlayBuffer, deltaTime);

        // uint8_t messageStatus = (uint8_t) ENoteEvent::NOTE_ON;
        // SaveMessageStatus(&writer, messageStatus);

        // ChannelEvent channelEvent;
        // channelEvent.channel = 0;
        // channelEvent.message = ENoteEvent::NOTE_ON;
        // channelEvent.param1 = 80; // key
        // channelEvent.param1 = 100; // velocity
        // SaveChannelEvent(writer, channelEvent);
        SaveNoteOn(notePlayBuffer, 0, 80, 100);

        SaveDeltaTime(&notePlayBuffer, deltaTime);
        SaveNoteOn(notePlayBuffer, 0, 80, 100);
        SaveDeltaTime(&notePlayBuffer, deltaTime);
        SaveNoteOn(notePlayBuffer, 0, 80, 100);
        SaveDeltaTime(&notePlayBuffer, deltaTime);
        SaveNoteOn(notePlayBuffer, 0, 80, 100);
        SaveDeltaTime(&notePlayBuffer, deltaTime);
        SaveNoteOn(notePlayBuffer, 0, 80, 100);

        {
            uint32_t deltaTime = 1000000;
            size_t t = SaveDeltaTime(&notePlayBuffer, deltaTime);
            SaveEndOfTrack(notePlayBuffer);
            // SaveNoteOn(notePlayBuffer, 0, 80, 100);
        }
    }



    TrackHeader trackHeader;
    trackHeader.type = 0x6B72544D; // "MTrk"
    trackHeader.length = notePlayBuffer.offset;
    SaveTrackHeaderToBuffer(&writer, trackHeader);

    // Track data
    writer.Save((const char*)notePlayBuffer.bufferStart, notePlayBuffer.offset);

    std::ofstream myfile;
    myfile.open ("writeTest.mid", std::fstream::binary|std::fstream::in | std::fstream::out | std::fstream::trunc);
    myfile.write((const char*) writer.bufferStart, writer.offset);
    myfile.close();


    AdvancedMIDIParser parser;
    parser.LoadFromBytes((const char*)writer.bufferStart, writer.offset);

    free(notePlayBuffer.bufferStart);
    free(writer.bufferStart);
}

int main()
{
    try 
    {
        WriteMIDI();
    }
    catch (const std::exception& e)
    {
        displayError(e.what());
        //std::cout << e.what() << std::endl;
    }
}