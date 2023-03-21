#pragma once

#include <string>

#include <AL/al.h>

namespace audio
{
    // TODO: move out sin generation into a clip builder
    // TODO: doesn't seem to work with length < 1
    // TODO: weird blip/noise towards the end of the sound
    class AudioClip
    {
    public:
        explicit AudioClip(const std::string& name, float length, float frequency, float amplitude);
        ~AudioClip();

        [[nodiscard]] ALuint raw() const noexcept { return _clip; }

    private:
        std::string _name;
        ALuint _clip;
    };
}
