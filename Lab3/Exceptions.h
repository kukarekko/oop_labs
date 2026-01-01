#pragma once
#include <stdexcept>

class SoundException : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

class CmdLineError : public SoundException {
public: using SoundException::SoundException;
};

class ConfigError : public SoundException {
public: using SoundException::SoundException;
};

class FormatError : public SoundException {
public: using SoundException::SoundException;
};

