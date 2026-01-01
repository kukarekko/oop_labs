#include "ProcessManager.h"
#include <iostream>
#include <memory>
#include <vector>
#include "config/ConfigParser.h"
#include "converters/include/ConverterFactory.h"
#include "wav/WavReader.h"
#include "wav/WavWriter.h"
#include "audio/VectorAudioStream.h"
#include "Exceptions.h"

std::vector<std::vector<int16_t>> g_inputStreams;

void ProcessManager::printHelp() {
    std::cout << "Sound Processor - Audio Processing Tool\n\n"
              << "Usage:\n"
              << "  sound_processor -c <config.txt> <output.wav> <input1.wav> [input2.wav ...]\n"
              << "  sound_processor -h\n\n"
              << "Options:\n"
              << "  -c    Process audio using configuration file\n"
              << "  -h    Show this help message\n\n"
              << "Available converters (use in config file):\n"
              << "  gain <factor>                  - Apply gain (volume multiplier)\n"
              << "  mute <start_sec> <end_sec>     - Mute audio between start and end times\n"
              << "  mix <$n> [offset_sec]          - Mix with input file $n (e.g., $2 for second input)\n"
              << "\nExamples:\n"
              << "  sound_processor -c config.txt output.wav input1.wav\n"
              << "  sound_processor -c config.txt output.wav input1.wav input2.wav\n";
}

void ProcessManager::process(const std::string& configPath, 
                             const std::string& outputPath,
                             const std::vector<std::string>& inputPaths) {
    
    if (inputPaths.empty()) {
        throw CmdLineError("At least one input file required");
    }

    // Чтение входных файлов
    std::vector<std::shared_ptr<AudioStream>> inputs;
    g_inputStreams.clear();

    for (const auto& path : inputPaths) {
        std::cout << "Reading: " << path << std::endl;
        auto samples = WavReader::read(path);
        std::cout << "  Samples: " << samples.size()
                  << " (" << (samples.size() / 44100.0) << " seconds)" << std::endl;

        if (samples.empty()) {
            throw FormatError("Input file is empty: " + path);
        }

        g_inputStreams.push_back(samples);
        inputs.push_back(std::make_shared<VectorAudioStream>(samples));
    }

    ConverterFactory factory;

    // Парсинг конфигурации
    auto commands = ConfigParser::parse(configPath);
    std::cout << "Found " << commands.size() << " command(s) in config" << std::endl;

    std::shared_ptr<AudioStream> current = inputs[0];

    // Применяем конвертеры через фабрику
    for (const auto& cmd : commands) {
        std::cout << "Applying: " << cmd.name;
        for (const auto& arg : cmd.args) {
            std::cout << " " << arg;
        }
        std::cout << std::endl;

        auto converter = factory.createConverter(cmd.name, cmd.args);
        current = converter->apply(current);
    }

    std::cout << "Writing output..." << std::endl;

    // Преобразовываем в вектор для записи
    std::vector<int16_t> outputData(current->size());
    for (size_t i = 0; i < current->size(); ++i) {
        outputData[i] = current->sample(i);
    }

    WavWriter::write(outputPath, outputData, true);

    std::cout << "Success! Output saved to: " << outputPath << std::endl;
    std::cout << "Duration: " << (outputData.size() / 44100.0) << " seconds" << std::endl;
}