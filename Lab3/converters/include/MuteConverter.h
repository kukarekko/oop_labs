#pragma once
#include "Converter.h"

class MuteConverter : public Converter {
public:
    MuteConverter(double s, double e);
    std::shared_ptr<AudioStream>
    apply(std::shared_ptr<AudioStream>) override;
    std::string help() const override;

private:
    size_t start, end;
};
