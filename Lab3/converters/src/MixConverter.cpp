#include <string>
#include <stdexcept>
#include <iostream>
#include "include/MixConverter.h"
#include "../../audio/VectorAudioStream.h"
#include "../../wav/WavReader.h"

MixConverter::MixConverter(const std::vector<int16_t>& o, double off)
    : add(o), offset(static_cast<size_t>(off * 44100)) {

    if (off < 0) {
        throw std::invalid_argument("Offset cannot be negative");
    }
    std::cout << "MixConverter created: offset=" << offset
              << " samples, other stream size=" << add.size()
              << " samples" << std::endl;
}

std::shared_ptr<AudioStream>
MixConverter::apply(std::shared_ptr<AudioStream> in) {
    if (!in) {
        throw std::invalid_argument("Input stream is null");
    }

    size_t inputSize = in->size();
    size_t totalMixedSize = inputSize;
    if (!add.empty()) {
        size_t otherStreamEnd = offset + add.size();
        totalMixedSize = std::max(inputSize, otherStreamEnd);
    }
    std::cout << "Mixing streams: inputSize=" << inputSize
              << ", totalMixedSize=" << totalMixedSize
              << ", addSize=" << add.size() << std::endl;

    auto out = std::vector<int16_t>(totalMixedSize, 0);

    for (size_t i = 0; i < inputSize; ++i) {
        out[i] = in->sample(i);
    }

    for (size_t i = 0; i < add.size(); ++i) {  //копируем второй поток
        size_t outputIndex = offset + i;

        if (outputIndex >= out.size()) {
            std::cout << "Warning: Output buffer resized to accommodate mixing" << std::endl;
            out.resize(outputIndex + 1, 0);
        }
        //смешиваем
        int32_t mixed = static_cast<int32_t>(out[outputIndex]) + static_cast<int32_t>(add[i]);
        mixed /= 2;

        //диапазон
        if (mixed > 32767) mixed = 32767;
        if (mixed < -32768) mixed = -32768;

        out[outputIndex] = static_cast<int16_t>(mixed);
    }

    std::cout << "Final mixed size: " << out.size()
              << " samples (" << (out.size() / 44100.0) << " seconds)" << std::endl;

    return std::make_shared<VectorAudioStream>(out);
}

std::string MixConverter::help() const {
    return "mix <$n> [offset] - Mix current stream with input $n starting at offset seconds (default: 0)";
}
