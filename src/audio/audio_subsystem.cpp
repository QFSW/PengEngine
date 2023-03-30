#include "audio_subsystem.h"

#include <core/logger.h>
#include <utils/check.h>

using namespace audio;

AudioSubsystem::AudioSubsystem()
    : Subsystem()
    , _device(nullptr)
    , _context(nullptr)
    , _active(false)
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
