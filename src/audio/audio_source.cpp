#include "audio_source.h"

#include "audio_clip.h"

using namespace audio;

AudioSource::AudioSource()
    : _state(AL_INITIAL)
{
    alGenSources(1, &_source);
}

AudioSource::~AudioSource()
{
    alDeleteSources(1, &_source);
}

void AudioSource::play(const peng::shared_ref<const AudioClip>& clip)
{
    if (is_playing())
    {
        stop();
    }

    alSourcei(_source, AL_BUFFER, clip->raw());
    alSourcePlay(_source);

    _current_clip = clip;
    _state = AL_PLAYING;
}

void AudioSource::stop()
{
    alSourceStop(_source);

    _state = AL_STOPPED;
    _current_clip = {};
}

bool AudioSource::is_playing()
{
    update_state();
    return _state == AL_PLAYING;
}

void AudioSource::update_state()
{
    alGetSourcei(_source, AL_SOURCE_STATE, &_state);

    if (_state != AL_PLAYING)
    {
        _current_clip = {};
    }
}
