#pragma once
#include <cstddef>
#include <cstdint>

class AudioStream {
public:
    virtual int16_t sample(size_t i) const = 0; //сэмпл по индексу
    virtual size_t size() const = 0;            //количество сэмплов
    virtual ~AudioStream() = default;           // деструктор
};