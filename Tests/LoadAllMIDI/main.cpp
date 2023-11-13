#include <filesystem>
#include <iostream>
#include <fstream>
#include <thread>
#include <future>
#include <atomic>
#include "MIDIParserException.h"
#include "LoggingMIDIParser.h"
#include "ConfigParser.h"

class LoadAllMIDI
{
    ConfigParser config = ConfigParser("config.txt");
    std::atomic<size_t> nbSuccess = 0;
    std::atomic<size_t> nbFailures = 0;

    using BatchType = std::vector<std::pair<size_t, std::filesystem::directory_entry>>;

public:
    void TryLoadFile(size_t i, const std::filesystem::directory_entry& entry);
    void TryLoadAllFiles(bool useMultithreads = true, size_t batchSize = 10000);
    void TryLoadBatch(BatchType batch);
};

void displayError(const std::string& s)
{
    std::cout << "\033[1;31m" << s << "\033[0m\n" << std::endl;    
}

void displaySuccess(const std::string& s)
{
    std::cout << "\033[1;32m" << s << "\033[0m\n" << std::endl;    
}

void LoadAllMIDI::TryLoadFile(size_t i, const std::filesystem::directory_entry& entry)
{
    std::cout << "Trying to load : " << i << " / " << entry.path().string() << std::endl;

    try 
    {
        
        LoggingMIDIParser parser("output.txt");

        std::ifstream file (entry.path(), std::ios::in|std::ios::binary|std::ios::ate);
        if (file.is_open())
        {
            size_t size = file.tellg();
            char* memblock = new char [size];
            file.seekg (0, std::ios::beg);
            file.read (memblock, size);
            file.close();

            parser.LoadFromBytes(memblock, size);
            // parser.OnLoadedFromFile(filename);

            delete[] memblock;
        }
        else
        {
            throw std::runtime_error("Couldn't open file : " + entry.path().string());
        }

        nbSuccess++;
        displaySuccess(std::to_string(nbSuccess) + ". Loaded with success! " + entry.path().string());
        // std::cout << "Loaded with success! " << entry.path().string() << std::endl;

        // std::ios::app is the open mode "append" meaning
        // new data will be written to the end of the file.
        std::ofstream out;
        out.open("config.txt", std::ios::app);

        config.parsedLines.emplace_back(entry.path().string(), "Success\n");
        // out << entry.path().string() << " : " << "Success\n";
    }
    catch (const MIDIParserException& e)
    {
        nbFailures++;
        displayError(std::to_string(nbFailures) + ". MIDIParserException : " + std::string(e.what()));
        // std::cout << "MIDIParserException : " << e.what() << std::endl;

        // std::ios::app is the open mode "append" meaning
        // new data will be written to the end of the file.
        std::ofstream out;
        out.open("config.txt", std::ios::app);
        config.parsedLines.emplace_back(entry.path().string(), "Failure\n");
        out.close();
        // out << entry.path().string() << " : " << "Failure\n";
    }
    catch (const std::exception& e)
    {
        nbFailures++;
        displayError(std::to_string(nbFailures) + ". std::exception : " + std::string(e.what()));
        // std::cout << "std::exception : " << e.what() << std::endl;

        // std::ios::app is the open mode "append" meaning
        // new data will be written to the end of the file.
        std::ofstream out;
        out.open("config.txt", std::ios::app);
        config.parsedLines.emplace_back(entry.path().string(), "Failure\n");
        out.close();
        // out << entry.path().string() << " : " << "Failure\n";
    }
}

void LoadAllMIDI::TryLoadBatch(BatchType batch)
{
    for (const auto& elem : batch)
    {
        TryLoadFile(elem.first, elem.second);
    }
}

void LoadAllMIDI::TryLoadAllFiles(bool useMultithreads, size_t batchSize)
{
    size_t nbFailures = 0;

    size_t i = 0;
    std::string path = "C:/Users/thoma/Documents/Projects/ModularMusicGenerationModules/Assets/Datasets/LakhMidi-full";

    std::vector<std::pair<size_t, std::filesystem::directory_entry>> batch;
    batch.reserve(batchSize);

    std::vector<decltype(std::async(&LoadAllMIDI::TryLoadBatch, this, std::move(batch)))> threads;
    auto runBatch = [&]()
    {
        if (useMultithreads)
        {
            threads.emplace_back(std::async(&LoadAllMIDI::TryLoadBatch, this, std::move(batch)));
        }
        else 
        {
            LoadAllMIDI::TryLoadBatch(std::move(batch));
        }

        batch.clear();
    };

    for (const auto& entry : std::filesystem::recursive_directory_iterator(path))
    {
        if (entry.is_directory() || entry.path().extension().string() != ".mid")
            continue;

        auto it = std::find_if(config.parsedLines.begin(), config.parsedLines.end(), ([&entry](const std::pair<std::string, std::string>& pair)
        {
            return pair.first == entry.path().string() && pair.second == "Success";
        }));

        i++;

        // corrupted file / crash : 
        // Tears_in_Heaven.7.mid
        //if (i == 5596)
        //    continue;

        // Crash, but is valid : 
        //  7300 / C:/Users/thoma/Downloads/archive\Jackson_Michael\Ill_Be_There.mid

        // if (i < 151)
        //     continue;
        if (it != config.parsedLines.end())
            continue;
        
        batch.emplace_back(i, entry);

        if (batch.size() >= batchSize)
        {
            runBatch();
        }
    }
    runBatch();
}

int main()
{
    try 
    {
        LoadAllMIDI loader;
        loader.TryLoadAllFiles();
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}