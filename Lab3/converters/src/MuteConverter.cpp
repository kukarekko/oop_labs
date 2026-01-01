#include "include/MuteConverter.h"
#include "../../audio/VectorAudioStream.h"
#include <string>

MuteConverter::MuteConverter(double s, double e) {
    start = s * 44100;
    end = e * 44100;
}

std::shared_ptr<AudioStream>
MuteConverter::apply(std::shared_ptr<AudioStream> in) {
    auto out = std::vector<int16_t>(in->size());
    for (size_t i = 0; i < in->size(); ++i)
        out[i] = (i >= start && i < end) ? 0 : in->sample(i);
    return std::make_shared<VectorAudioStream>(out);
}

std::string MuteConverter::help() const {
    return "mute <start> <end>";
}
