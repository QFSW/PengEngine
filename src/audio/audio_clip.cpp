#include "audio_clip.h"

#include <core/logger.h>
#include <core/archive.h>
#include <profiling/scoped_event.h>

#include "audio_decoder.h"
#include "raw_audio_data.h"

using namespace audio;

AudioClip::AudioClip(const std::string& name, const RawAudioData& raw_audio)
    : _name(name)
{
    SCOPED_EVENT("Building audio clip", _name.c_str());
    Logger::log("Building audio clip '%s'", _name.c_str());

    raw_audio.check_valid();
    alGenBuffers(1, &_clip);
    alBufferData(_clip, raw_audio.format(), raw_audio.samples.data(), static_cast<ALsizei>(raw_audio.samples.size()), raw_audio.sample_rate);
}

AudioClip::AudioClip(const std::string& name, const std::string& audio_path)
    : AudioClip(
        name,
        AudioDecoder::load_file(audio_path)
    )
{ }

AudioClip::~AudioClip()
{
    SCOPED_EVENT("Destroying audio clip", _name.c_str());
    Logger::log("Destroying audio clip '%s'", _name.c_str());

    alDeleteBuffers(1, &_clip);
}

peng::shared_ref<AudioClip> AudioClip::load_asset(const Archive& archive)
{
    const std::string audio_path = archive.read<std::string>("audio");
    return peng::make_shared<AudioClip>(archive.name, audio_path);
}
