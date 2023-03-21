#pragma once

#include <AL/al.h>
#include <AL/alc.h>

namespace audio
{
    class AudioSubsystem
    {
    public:
        AudioSubsystem();
        ~AudioSubsystem();

        void start();
        void shutdown();

    private:
        ALCdevice* _device;
        ALCcontext* _context;

        bool _active;
    };
}