#include "audio_subsystem.h"

#include <cassert>

#include <core/logger.h>

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

    assert(!_active);
    _active = true;

    _device = alcOpenDevice(nullptr);
    _context = alcCreateContext(_device, nullptr);
    alcMakeContextCurrent(_context);
}

void AudioSubsystem::shutdown()
{
    Logger::log("Shutting down OpenAL");

    assert(_active);
    _active = false;

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(_context);
    alcCloseDevice(_device);
}

void AudioSubsystem::tick(float)
{
}
