#pragma once

#include "Macros.h"
#include <vector>
#include <cstdint>

struct MIDIPARSEREXPORT Tokenizer
{
public:
    std::vector<float> tokens;
    class MIDIMusic* music;

public:
    Tokenizer(class MIDIMusic* newMusic) : music(newMusic) {}
    uint32_t GetNbTokens() const {return tokens.size();}
    float* GetTokens() {return tokens.data();}
};

extern "C"
{
    MIDIPARSEREXPORT Tokenizer* Tokenizer_Create(class MIDIMusic* music);
    MIDIPARSEREXPORT void Tokenizer_Destroy(Tokenizer* tokenizer);

    MIDIPARSEREXPORT uint32_t Tokenizer_GetNbTokens(Tokenizer* tokenizer);
    MIDIPARSEREXPORT float* Tokenizer_GetTokens(Tokenizer* tokenizer);

    // Usage: 
    // auto* tok = Tokenizer_Create();
    // Tokenizer_BuildTokensFromNotes1(tok);
    // Tokenizer_Destroy(tok);
    MIDIPARSEREXPORT void Tokenizer_BuildTokensFromNotesHelper(Tokenizer* tokenizer, float (*GetTokenFromNotes)(uint32_t nbNotes, uint32_t* notes));

    // Convert a set of notes to a token
    // Used for notes that happen at the same time ; might not be compatible with some tokenization methods
    MIDIPARSEREXPORT float GetTokenFromNotes1(uint32_t nbNotes, uint32_t* notes);
    MIDIPARSEREXPORT void Tokenizer_BuildTokensFromNotes1(Tokenizer* tokenizer);
}
