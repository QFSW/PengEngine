#pragma once

namespace audio
{
    struct RawAudioData
    {
        bool corrupt = false;

        static RawAudioData corrupt_data();
    };
}