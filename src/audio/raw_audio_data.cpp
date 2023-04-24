#include "raw_audio_data.h"

using namespace audio;

RawAudioData RawAudioData::corrupt_data()
{
    return RawAudioData{
        .corrupt = true
    };
}
