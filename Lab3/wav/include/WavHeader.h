#pragma once
#include <cstdint>

struct WavHeader {
    char riff[4];
    uint32_t chunkSize;
    char wave[4];
    char fmt[4];
    uint32_t subchunk1Size;
    uint16_t audioFormat;
    uint16_t numChannels;
    uint32_t sampleRate; //сэмплов в секунду
    uint32_t byteRate;   //sampleRate * numChannels * bitsPerSample / 8
    uint16_t blockAlign; //Сколько байт занимает один сэмпл
    uint16_t bitsPerSample; // 16
    char data[4];
    uint32_t dataSize;
};
