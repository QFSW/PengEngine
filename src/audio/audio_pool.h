#pragma once

#include "audio_source.h"

namespace audio
{
    class AudioClip;

    // TODO: need a max pool size and to manage that accordingly
    // TODO: should probably be a singleton eventually
    class AudioPool
    {
    public:
        void play(const peng::shared_ref<const AudioClip>& clip);

    private:
        peng::shared_ref<AudioSource> get_free_source();

        std::vector<peng::shared_ptr<AudioSource>> _free_sources;
        std::vector<peng::shared_ptr<AudioSource>> _busy_sources;
    };
}
