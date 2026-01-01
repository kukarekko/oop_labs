#pragma once
#include "Converter.h"
#include <vector>

class MixConverter : public Converter {
public:
    MixConverter(const std::vector<int16_t>& other, double offset);
    std::shared_ptr<AudioStream>
    apply(std::shared_ptr<AudioStream>) override;
    std::string help() const override;

private:
    std::vector<int16_t> add;
    size_t offset;
};
