#pragma once
#include <memory>
#include <string>
#include <vector>
#include "audio/AudioStream.h"

class ProcessManager {
public:
	static void printHelp();
	void process(const std::string& configPath,
				 const std::string& outputPath,
				 const std::vector<std::string>& inputPaths);
};
