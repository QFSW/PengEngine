#include "audio_subsystem.h"

#include <core/logger.h>
#include <utils/check.h>

using namespace audio;

AudioSubsystem::AudioSubsystem()
    : Subsystem()
    , _device(nullptr)
    , _context(nullptr)
    , _active(false)
    , _volume(1)
{ }

AudioSubsystem::~AudioSubsystem()
{
    if (_active)
    {
        shutdown();
    }
}

void AudioSubsystem::start()
{
    Logger::log("Starting OpenAL");

    check(!_active);
    _active = true;

    _device = alcOpenDevice(nullptr);
    _context = alcCreateContext(_device, nullptr);
    alcMakeContextCurrent(_context);

    set_volume(_volume);
}

void AudioSubsystem::shutdown()
{
    Logger::log("Shutting down OpenAL");

    check(_active);
    _active = false;

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(_context);
    alcCloseDevice(_device);
}

void AudioSubsystem::tick(float)
{
}

void AudioSubsystem::set_volume(float volume)
{
    check(volume >= 0 && volume <= 1);
    _volume = volume;

    if (_active)
    {
        alListenerf(AL_GAIN, volume);
    }
}

float AudioSubsystem::volume() const noexcept
{
    return _volume;
}
