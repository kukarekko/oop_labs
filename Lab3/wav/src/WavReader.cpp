#include "include/WavReader.h"
#include "Exceptions.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>

std::vector<int16_t> WavReader::read(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) {
        throw FormatError("Cannot open WAV file: " + path);
    }

    //Проверка размера файла
    in.seekg(0, std::ios::end);
    size_t fileSize = in.tellg();
    in.seekg(0, std::ios::beg);

    if (fileSize < 44) {
        throw FormatError("File too small to be a WAV: " + path);
    }

    // Читаем RIFF заголовок
    char riff[5] = {0}, wave[5] = {0};
    in.read(riff, 4);
    uint32_t totalSize;
    in.read(reinterpret_cast<char*>(&totalSize), 4);
    in.read(wave, 4);

    if (std::string(riff) != "RIFF" || std::string(wave) != "WAVE") {
        throw FormatError("Invalid WAV file signature in: " + path);
    }

    // Ищем чанки
    bool foundFmt = false, foundData = false;
    uint32_t sampleRate = 44100;
    uint16_t numChannels = 1;
    uint16_t bitsPerSample = 16;
    uint32_t dataSize = 0;
    std::streampos dataStart = 0;

    while (in.good() && !foundData) {
        char chunkId[5] = {0};
        uint32_t chunkSize;

        in.read(chunkId, 4);
        in.read(reinterpret_cast<char*>(&chunkSize), 4);

        if (!in.good()) break;

        if (std::string(chunkId) == "fmt ") {
            foundFmt = true;

            uint16_t audioFormat;
            in.read(reinterpret_cast<char*>(&audioFormat), 2);
            in.read(reinterpret_cast<char*>(&numChannels), 2);
            in.read(reinterpret_cast<char*>(&sampleRate), 4);

            uint32_t byteRate;
            uint16_t blockAlign;

            in.read(reinterpret_cast<char*>(&byteRate), 4);
            in.read(reinterpret_cast<char*>(&blockAlign), 2);
            in.read(reinterpret_cast<char*>(&bitsPerSample), 2);

            // Проверки формата
            if (audioFormat != 1) {
                throw FormatError("Only PCM format supported. File has format: " + std::to_string(audioFormat));
            }
            if (numChannels != 1) {
                throw FormatError("Only mono audio supported. File has " + std::to_string(numChannels) + " channels");
            }
            if (sampleRate != 44100) {
                throw FormatError("Only 44100 Hz supported. File has sample rate: " + std::to_string(sampleRate) + " Hz");
            }
            if (bitsPerSample != 16) {
                throw FormatError("Only 16-bit samples supported. File has " + std::to_string(bitsPerSample) + " bits");
            }

            // Пропускаем оставшиеся байты чанка
            if (chunkSize > 16) {
                in.seekg(chunkSize - 16, std::ios::cur);
            }
        }
        else if (std::string(chunkId) == "data") {
            foundData = true;
            dataSize = chunkSize;
            dataStart = in.tellg();
            in.seekg(chunkSize, std::ios::cur);
        }
        else {
            in.seekg(chunkSize, std::ios::cur);
        }
    }

    if (!foundFmt) {
        throw FormatError("fmt chunk not found in: " + path);
    }

    if (!foundData) {
        throw FormatError("data chunk not found in: " + path);
    }

    in.clear();
    in.seekg(dataStart);

    size_t sampleCount = dataSize / (bitsPerSample / 8); //количество сэмплов
    if (numChannels > 1) {
        sampleCount /= numChannels;
    }

    std::vector<int16_t> data(sampleCount);

    if (numChannels == 1 && bitsPerSample == 16) {
        in.read(reinterpret_cast<char*>(data.data()), dataSize);
    }
    else if (numChannels == 2 && bitsPerSample == 16) { // Стерео -> моно: усредняем каналы
        std::vector<int16_t> stereo(dataSize / 2);
        in.read(reinterpret_cast<char*>(stereo.data()), dataSize);

        for (size_t i = 0, j = 0; i < stereo.size(); i += 2, j++) {
            int32_t left = stereo[i];
            int32_t right = stereo[i + 1];
            data[j] = static_cast<int16_t>((left + right) / 2);
        }
    }
    else {
        throw FormatError("Unsupported audio format in: " + path);
    }

    if (!in.good()) {
        throw FormatError("Could not read all audio data from: " + path);
    }

    return data;
}