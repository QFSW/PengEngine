#pragma once

#include <string>

#include <AL/al.h>
#include <memory/shared_ref.h>

struct Archive;

namespace audio
{
    struct RawAudioData;

    class AudioClip
    {
    public:
        AudioClip(const std::string& name, const RawAudioData& raw_audio);
        AudioClip(const std::string& name, const std::string& audio_path);
        ~AudioClip();

        static peng::shared_ref<AudioClip> load_asset(const Archive& archive);

        [[nodiscard]] ALuint raw() const noexcept { return _clip; }

    private:
        std::string _name;
        ALuint _clip;
    };
}
