cmake_minimum_required(VERSION 3.10)

project(sound_processor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(sound_processor
        main.cpp
        process/ProcessManager.cpp

        # Audio
        audio/VectorAudioStream.cpp

        # Config
        config/ConfigParser.cpp

        # Converters
        converters/src/ConverterFactory.cpp
        converters/src/MuteConverter.cpp
        converters/src/MixConverter.cpp
        converters/src/GainConverter.cpp

        # WAV
        wav/WavReader.cpp
        wav/WavWriter.cpp
)

# Включаем директории
target_include_directories(sound_processor PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/audio
        ${CMAKE_CURRENT_SOURCE_DIR}/config
        ${CMAKE_CURRENT_SOURCE_DIR}/converters
        ${CMAKE_CURRENT_SOURCE_DIR}/wav
        ${CMAKE_CURRENT_SOURCE_DIR}/exceptions
)


