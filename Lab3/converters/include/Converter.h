#pragma once
#include <memory>
#include "../audio/AudioStream.h"

class Converter {
public:
    virtual std::shared_ptr<AudioStream>
    apply(std::shared_ptr<AudioStream> in) = 0;

    virtual std::string help() const = 0;
    virtual ~Converter() = default;
};
