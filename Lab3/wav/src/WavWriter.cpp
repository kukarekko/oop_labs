#include "include/WavWriter.h"
#include "../../Exceptions.h"
#include <fstream>
#include <iostream>

void WavWriter::write(const std::string& filename,
                     const std::vector<int16_t>& samples,
                     bool isFirstWrite) {

    std::ofstream file(
        filename,
        std::ios::binary | (isFirstWrite ? std::ios::out : std::ios::app));

    if (!file.is_open()) {
        throw FormatError("Failed to open file: " + filename);
    }

    if (isFirstWrite) {
        file.write("RIFF", 4);
        uint32_t file_size = 36 + samples.size() * 2;
        file.write(reinterpret_cast<const char*>(&file_size), 4);
        file.write("WAVE", 4);
        file.write("fmt ", 4);
        uint32_t fmt_size = 16;
        file.write(reinterpret_cast<const char*>(&fmt_size), 4);
        uint16_t audio_format = 1;
        file.write(reinterpret_cast<const char*>(&audio_format), 2);
        uint16_t num_channels = 1;
        file.write(reinterpret_cast<const char*>(&num_channels), 2);
        uint32_t sample_rate = 44100;
        file.write(reinterpret_cast<const char*>(&sample_rate), 4);
        uint32_t byte_rate = 44100 * 2;
        file.write(reinterpret_cast<const char*>(&byte_rate), 4);
        uint16_t block_align = 2;
        file.write(reinterpret_cast<const char*>(&block_align), 2);
        uint16_t bits_per_sample = 16;
        file.write(reinterpret_cast<const char*>(&bits_per_sample), 2);
        file.write("data", 4);
        uint32_t data_size = samples.size() * 2;
        file.write(reinterpret_cast<const char*>(&data_size), 4);
    }

    // Запись аудиоданных
    file.write(reinterpret_cast<const char*>(samples.data()),
               samples.size() * sizeof(int16_t));

    // Если не первая запись, обновляем размеры в заголовке
    if (!isFirstWrite) {
        file.close();

        std::fstream update_file(
            filename, std::ios::in | std::ios::out | std::ios::binary);

        if (!update_file.is_open()) {
            throw FormatError("Failed to open file for updating header: " + filename);
        }

        update_file.seekg(0, std::ios::end);
        uint32_t total_file_size = static_cast<uint32_t>(update_file.tellg()) - 8;
        uint32_t total_data_size = total_file_size - 36;

        update_file.seekp(4, std::ios::beg);
        update_file.write(reinterpret_cast<const char*>(&total_file_size), 4);

        update_file.seekp(40, std::ios::beg);
        update_file.write(reinterpret_cast<const char*>(&total_data_size), 4);

        update_file.close();
    }

    std::cout << "Wrote " << samples.size() << " samples to " << filename
              << " (" << (samples.size() / 44100.0) << " seconds)" << std::endl;
}
