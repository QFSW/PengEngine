#pragma once

#include <cstdint>
#include <vector>

#include <AL/al.h>

namespace audio
{
    struct RawAudioData
    {
        uint32_t num_channels = 0;
        uint32_t sample_rate = 0;
        uint16_t bits_per_sample = 0;
        std::vector<uint8_t> samples;

        bool corrupt = false;

        void check_valid() const;
        [[nodiscard]] ALenum format() const noexcept;

        static RawAudioData corrupt_data();
    };
}
