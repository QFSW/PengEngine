#include "audio_clip.h"

#include <core/logger.h>
#include <profiling/scoped_event.h>
#include <math/math.h>

using namespace audio;

AudioClip::AudioClip(const std::string& name, float length, float frequency, float amplitude)
    : _name(name)
{
    SCOPED_EVENT("Building audio clip", _name.c_str());
    Logger::log("Building audio clip '%s'", _name.c_str());

    // TODO: this is dummy data
    constexpr int32_t sample_rate = 44100;

    const size_t num_samples = static_cast<size_t>(length * static_cast<float>(sample_rate));
    std::vector<int16_t> samples;
    samples.resize(num_samples);

    for (size_t i = 0; i < num_samples; i++)
    {
        const float t = i / static_cast<float>(sample_rate);
        const float a = math::map(t, math::Vector2f(0, length), math::Vector2f(1, 0));
        const float val_f = std::sin(t * 6.2f * frequency);
        const float val_m = val_f * amplitude * a * a * a;

        constexpr int16_t max_val = std::numeric_limits<int16_t>::max();
        samples[i] = static_cast<int16_t>(math::map(val_m, math::Vector2f(-1, 1), math::Vector2f(-max_val, max_val)));
    }

    alGenBuffers(1, &_clip);
    alBufferData(_clip, AL_FORMAT_MONO16, samples.data(), static_cast<ALsizei>(samples.size()), sample_rate);
}

AudioClip::~AudioClip()
{
    SCOPED_EVENT("Destroying audio clip", _name.c_str());
    Logger::log("Destroying audio clip '%s'", _name.c_str());

    alDeleteBuffers(1, &_clip);
}
