#pragma once
#include <string>
#include <vector>
#include <cstdint>

class WavReader {
public:
    struct WavHeader {
        char riff[4];
        uint32_t fileSize;
        char wave[4];
        char fmt[4];
        uint32_t fmtSize;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;
    };

    static std::vector<int16_t> read(const std::string& path);
};
