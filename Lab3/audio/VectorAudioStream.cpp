#include "VectorAudioStream.h"

VectorAudioStream::VectorAudioStream(std::vector<int16_t> data)
    : samples(std::move(data)) {}

int16_t VectorAudioStream::sample(size_t i) const {
    return samples.at(i);
}

size_t VectorAudioStream::size() const {
    return samples.size();
}

std::vector<int16_t>& VectorAudioStream::raw() {
    return samples;
}
