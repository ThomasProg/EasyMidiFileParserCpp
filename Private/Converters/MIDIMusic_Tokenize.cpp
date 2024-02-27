#include "Converters/MIDIMusic_Tokenize.h"

#include <iostream>
#include <vector>
#include <memory>
#include <cassert>
#include "PMIDIEvent.h"
#include "MIDIMusic.h"
#include "Converters/MIDIMusic_AbsoluteConverter.h"
#include "Converters/MIDIMusic_RelativeConverter.h"
#include "Converters/MIDIMusic_CompressorConverter.h"

Tokenizer* Tokenizer_Create(class MIDIMusic* music)
{
    return new Tokenizer(music);
}
void Tokenizer_Destroy(Tokenizer* tokenizer)
{
    delete tokenizer;
}

uint32_t Tokenizer_GetNbTokens(Tokenizer* tokenizer)
{
    return tokenizer->GetNbTokens();
}

float* Tokenizer_GetTokens(Tokenizer* tokenizer)
{
    return tokenizer->GetTokens();
}

std::vector<std::vector<uint32_t>> getKeysPerTick(class MIDIMusic& newMusic)
{
    std::vector<std::vector<uint32_t>> keysPerTick; 
    keysPerTick.emplace_back();

    for (std::shared_ptr<PMIDIEvent>& e : newMusic.tracks[0].midiEvents)
    {
        for (uint32_t i = 0; i < e->deltaTime; i++)
        {
            keysPerTick.emplace_back();
        }

        if (std::shared_ptr<NoteOnOff> noteOnOff = std::dynamic_pointer_cast<NoteOnOff>(e))
        {
            keysPerTick.back().push_back(noteOnOff->key);
        }
    }

    return keysPerTick;
}

void Tokenizer_BuildTokensFromNotesHelper(Tokenizer* tokenizer, float (*GetTokenFromNotes)(uint32_t nbNotes, uint32_t* notes))
{
    assert(tokenizer->music->tracks.size() == 1);

    std::vector<std::vector<uint32_t>> keysPerTick = getKeysPerTick(*tokenizer->music);

    tokenizer->tokens.clear();
    for (auto& v : keysPerTick)
    {
        tokenizer->tokens.push_back(GetTokenFromNotes(v.size(), v.data()));
    }
}

float GetTokenFromNotes1(uint32_t nbNotes, uint32_t* notes)
{
    if (nbNotes > 0)
    {
        return 1.f;
    }
    else
    {
        return 0.f;
    }
}

void Tokenizer_BuildTokensFromNotes1(Tokenizer* tokenizer)
{
    Tokenizer_BuildTokensFromNotesHelper(tokenizer, GetTokenFromNotes1);
}