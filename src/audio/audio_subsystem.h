#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <core/subsystem.h>

namespace audio
{
    class AudioSubsystem final : public Subsystem
    {
        DECLARE_SUBSYSTEM(AudioSubsystem)

    public:
        AudioSubsystem();
        ~AudioSubsystem() override;

        void start() override;
        void shutdown() override;
        void tick(float delta_time) override;

    private:
        ALCdevice* _device;
        ALCcontext* _context;

        bool _active;
    };
}