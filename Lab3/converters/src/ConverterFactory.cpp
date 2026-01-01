#include "include/ConverterFactory.h"
#include "Exceptions.h"
#include "include/GainConverter.h"
#include "include/MuteConverter.h"
#include "include/MixConverter.h"
#include <iostream>

extern std::vector<std::vector<int16_t>> g_inputStreams;

std::unique_ptr<Converter> GainConverterCreator::createConverter(const std::vector<std::string>& args) {
    if (args.size() != 1) {
        throw ConfigError("gain requires 1 argument: factor");
    }
    double factor = std::stod(args[0]);
    if (factor < 0) {
        throw ConfigError("Gain factor must be non-negative");
    }
    return std::make_unique<GainConverter>(factor);
}

std::string GainConverterCreator::getConverterName() const {
    return "gain";
}

std::unique_ptr<Converter> MuteConverterCreator::createConverter(const std::vector<std::string>& args) {
    if (args.size() != 2) {
        throw ConfigError("mute requires 2 arguments: start end");
    }
    double start = std::stod(args[0]);
    double end = std::stod(args[1]);
    if (start < 0 || end < start) {
        throw ConfigError("Invalid time range for mute");
    }
    return std::make_unique<MuteConverter>(start, end);
}

std::string MuteConverterCreator::getConverterName() const {
    return "mute";
}

std::unique_ptr<Converter> MixConverterCreator::createConverter(const std::vector<std::string>& args) {
    if (args.size() < 1 || args.size() > 2) {
        throw ConfigError("mix requires 1 or 2 arguments: $n [offset]");
    }

    if (args[0].empty() || args[0][0] != '$') {
        throw ConfigError("First argument of mix must be a reference like $1, $2, etc.");
    }

    int streamNum;
    try {
        streamNum = std::stoi(args[0].substr(1));
    } catch (...) {
        throw ConfigError("Invalid stream reference format: " + args[0]);
    }

    if (streamNum < 1 || streamNum > static_cast<int>(g_inputStreams.size())) {
        throw ConfigError("Stream reference out of range: " + args[0] +
                         " (only " + std::to_string(g_inputStreams.size()) + " inputs available)");
    }

    double offset = 0.0;
    if (args.size() == 2) {
        offset = std::stod(args[1]);
        if (offset < 0) {
            throw ConfigError("Offset cannot be negative");
        }
    }

    const std::vector<int16_t>& otherStream = g_inputStreams[streamNum - 1];
    return std::make_unique<MixConverter>(otherStream, offset);
}

std::string MixConverterCreator::getConverterName() const {
    return "mix";
}

void ConverterFactory::registerCreator(std::unique_ptr<ConverterCreator> creator) {
    creators[creator->getConverterName()] = std::move(creator);
}

ConverterFactory::ConverterFactory() {
    registerCreator(std::make_unique<GainConverterCreator>());
    registerCreator(std::make_unique<MuteConverterCreator>());
    registerCreator(std::make_unique<MixConverterCreator>());
}

std::unique_ptr<Converter> ConverterFactory::createConverter(
    const std::string& converterName,
    const std::vector<std::string>& args) {

    auto it = creators.find(converterName);
    if (it == creators.end()) {
        throw ConfigError("Unknown converter: " + converterName);
    }

    return it->second->createConverter(args);
}

bool ConverterFactory::hasConverter(const std::string& name) const {
    return creators.find(name) != creators.end();
}
