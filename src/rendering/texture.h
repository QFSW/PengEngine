#pragma once

#include <string>
#include <vector>

#include <math/vector4.h>
#include <GL/glew.h>

namespace rendering
{
    class Texture
    {
    public:
        struct Config
        {
            GLint wrap_x = GL_REPEAT;
            GLint wrap_y = GL_REPEAT;
            GLint min_filter = GL_LINEAR_MIPMAP_LINEAR;
            GLint max_filter = GL_LINEAR;

            bool generate_mipmaps = true;
        };

        Texture(const std::string& name, const std::string& texture_path, const Config& config = {});

        Texture(
            const std::string& name,
            const std::vector<math::Vector3u8>& rgb_data,
            const math::Vector2i& resolution,
            const Config& config = {}
        );

        Texture(
            const std::string& name,
            const std::vector<math::Vector4u8>& rgba_data,
            const math::Vector2i& resolution,
            const Config& config = {}
        );

        // Creates an uninitialized texture with the specified resolution and channels
        Texture(
            const std::string& name,
            int32_t num_channels,
            const math::Vector2i& resolution,
            const Config& config = {}
        );

        Texture(const Texture&) = delete;
        Texture(Texture&&) = delete;
        ~Texture();

        void bind(GLint slot) const;
        void unbind(GLint slot) const;

        [[nodiscard]] GLuint raw() const noexcept;
        [[nodiscard]] math::Vector2i resolution() const noexcept;

    private:
        void verify_resolution(const math::Vector2i& resolution, int32_t num_pixels) const;
        void build_from_buffer(const void* texture_data);

        std::string _name;
        GLuint _tex;
        math::Vector2i _resolution;
        int32_t _num_channels;
        Config _config;
    };
}
