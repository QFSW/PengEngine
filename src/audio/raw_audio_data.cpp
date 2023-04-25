#include "raw_audio_data.h"

#include <utils/check.h>

using namespace audio;

void RawAudioData::check_valid() const
{
    check(num_channels > 0);
    check(sample_rate > 0);
    check(bits_per_sample > 0);
    check(bits_per_sample % 8 == 0);

    check(!corrupt);
    check(format() != AL_INVALID_ENUM);
}

ALenum RawAudioData::format() const noexcept
{
    if (num_channels == 1)
    {
        switch (bits_per_sample)
        {
        case 8:     return AL_FORMAT_MONO8;
        case 16:    return AL_FORMAT_MONO16;
        default:    return AL_INVALID_ENUM;
        }
    }

    if (num_channels == 2)
    {
        switch (bits_per_sample)
        {
        case 8:     return AL_FORMAT_STEREO8;
        case 16:    return AL_FORMAT_STEREO16;
        default:    return AL_INVALID_ENUM;
        }
    }

    return AL_INVALID_ENUM;
}

RawAudioData RawAudioData::corrupt_data()
{
    return RawAudioData{
        .corrupt = true
    };
}
