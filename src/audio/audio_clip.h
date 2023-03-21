#pragma once

#include <string>

#include <AL/al.h>

namespace audio
{
    class AudioClip
    {
    public:
        explicit AudioClip(const std::string& name);
        ~AudioClip();

    private:
        std::string _name;
        ALuint _clip;
    };
}
