#pragma once
#include "AudioStream.h"
#include <vector>

class VectorAudioStream : public AudioStream {
public:
    explicit VectorAudioStream(std::vector<int16_t> data);

    int16_t sample(size_t i) const override;
    size_t size() const override;
    std::vector<int16_t>& raw(); //Возвращает ссылку и на внутренний вектор samples

private:
    std::vector<int16_t> samples;
};