#include "audio_clip.h"

#include <core/logger.h>
#include <profiling/scoped_event.h>
#include <math/math.h>

using namespace audio;

AudioClip::AudioClip(const std::string& name)
    : _name(name)
{
    SCOPED_EVENT("Building audio clip", _name.c_str());
    Logger::log("Building audio clip '%s'", _name.c_str());

    // TODO: this is dummy data
    constexpr int32_t sample_rate = 44100;
    constexpr float frequency = 8000;
    constexpr float length = 0.5f;

    const size_t num_samples = length * sample_rate;
    std::vector<uint8_t> samples;
    samples.resize(num_samples);

    for (size_t i = 0; i < num_samples; i++)
    {
        const float t = i * length / sample_rate;
        const float val_f = std::sin(t * frequency / 6.2f);

        samples[i] = static_cast<uint8_t>(math::map(val_f, math::Vector2f(-1, 1), math::Vector2f(0, 0xFF)));
    }

    alGenBuffers(1, &_clip);
    alBufferData(_clip, AL_FORMAT_MONO8, samples.data(), static_cast<ALsizei>(samples.size()), sample_rate);
}

AudioClip::~AudioClip()
{
    SCOPED_EVENT("Destroying audio clip", _name.c_str());
    Logger::log("Destroying audio clip '%s'", _name.c_str());

    alDeleteBuffers(1, &_clip);
}
