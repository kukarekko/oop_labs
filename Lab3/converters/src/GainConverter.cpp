#include "include/GainConverter.h"
#include "../../audio/VectorAudioStream.h"
#include <string>
#include <algorithm>

GainConverter::GainConverter(double g) : gain(g) {}

std::shared_ptr<AudioStream>
GainConverter::apply(std::shared_ptr<AudioStream> in) {
    auto out = std::vector<int16_t>(in->size());
    for (size_t i = 0; i < in->size(); ++i) {
        int v = in->sample(i) * gain;
        out[i] = std::clamp(v, -32768, 32767); //ограничиваем результат, чтобы не было искажений
    }
    return std::make_shared<VectorAudioStream>(out);
}

std::string GainConverter::help() const {
    return "gain <factor>";
}
