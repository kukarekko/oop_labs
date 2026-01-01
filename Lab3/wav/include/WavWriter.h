#pragma once
#include <string>
#include <vector>
#include <cstdint>

class WavWriter {
public:
	static void write(const std::string& filename,
					 const std::vector<int16_t>& samples,
					 bool isFirstWrite = true);
};