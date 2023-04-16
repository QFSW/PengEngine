#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#include <core/subsystem.h>

namespace audio
{
    // TODO: OpenAL crashes on shutdown seemingly randomly
    class AudioSubsystem final : public Subsystem
    {
        DECLARE_SUBSYSTEM(AudioSubsystem)

    public:
        AudioSubsystem();
        ~AudioSubsystem() override;

        void start() override;
        void shutdown() override;
        void tick(float delta_time) override;

        void set_volume(float volume);
        [[nodiscard]] float volume() const noexcept;

    private:
        ALCdevice* _device;
        ALCcontext* _context;

        bool _active;
        float _volume;
    };
}