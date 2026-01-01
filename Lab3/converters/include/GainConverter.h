#pragma once
#include "Converter.h"

class GainConverter : public Converter {
public:
    explicit GainConverter(double g);
    std::shared_ptr<AudioStream>
    apply(std::shared_ptr<AudioStream>) override;
    std::string help() const override;

private:
    double gain;
};
