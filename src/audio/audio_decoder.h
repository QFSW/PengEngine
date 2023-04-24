#pragma once

#include <string>

#include "raw_audio_data.h"

namespace audio
{
    class AudioDecoder
    {
    public:
        static RawAudioData load_file(const std::string& path);

    private:
        static RawAudioData load_wave(const std::string& path);
    };
}