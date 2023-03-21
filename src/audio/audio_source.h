#pragma once

#include <AL/al.h>
#include <memory/shared_ptr.h>

namespace audio
{
    class AudioClip;

    class AudioSource
    {
    public:
        AudioSource();
        ~AudioSource();

        void play(const peng::shared_ref<const AudioClip>& clip);
        void stop();

        bool is_playing();

    private:
        void update_state();

        ALuint _source;
        ALenum _state;
        peng::shared_ptr<const AudioClip> _current_clip;
    };
}
