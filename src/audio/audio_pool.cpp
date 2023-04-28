#include "audio_pool.h"

using namespace audio;

void AudioPool::play(const peng::shared_ref<const AudioClip>& clip)
{
    get_free_source()->play(clip);
}

peng::shared_ref<AudioSource> AudioPool::get_free_source()
{
    // Promote busy sources to free sources first
    for (int32_t i = static_cast<int32_t>(_busy_sources.size() - 1); i >= 0; i--)
    {
        peng::shared_ptr<AudioSource> busy_source = _busy_sources[i];
        if (!busy_source->is_playing())
        {
            _free_sources.push_back(busy_source);
            _busy_sources.erase(_busy_sources.begin() + i);
        }
    }

    // Take a free source if there are any
    if (!_free_sources.empty())
    {
        peng::shared_ptr<AudioSource> free_source = _free_sources[_free_sources.size() - 1];
        _free_sources.erase(_free_sources.end() - 1);
        _busy_sources.push_back(free_source);

        return free_source.to_shared_ref();
    }

    // Create a new source otherwise
    peng::shared_ref<AudioSource> source = peng::make_shared<AudioSource>();
    _busy_sources.emplace_back(source);

    return source;
}
