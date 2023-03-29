#include "texture.h"

#include <stdexcept>

#include <core/logger.h>
#include <utils/strtools.h>
#include <common/common.h>
#include <profiling/scoped_event.h>

#pragma warning( push, 0 )
#define STB_IMAGE_IMPLEMENTATION
#include <libs/stb/stb_image.h>
#pragma warning( pop )

using namespace rendering;

Texture::Texture(const std::string& name, const std::string& texture_path, const Config& config)
    : _name(name)
    , _config(config)
{
    SCOPED_EVENT("Building texture", _name.c_str());
    Logger::log("Building texture '%s'", _name.c_str());
    Logger::log("Loading texture data '%s'", texture_path.c_str());

    stbi_set_flip_vertically_on_load(true);
    stbi_uc* texture_data = stbi_load(texture_path.c_str(), &_resolution.x, &_resolution.y, &_num_channels, 0);
    if (!texture_data)
    {
        throw std::runtime_error(strtools::catf("Could not load texture at %s", texture_path.c_str()));
    }

    build_from_buffer(texture_data);
    stbi_image_free(texture_data);
}

Texture::Texture(
    const std::string& name,
    const std::vector<math::Vector3u8>& rgb_data,
    const math::Vector2i& resolution,
    const Config& config
)
    : _name(name)
    , _resolution(resolution)
    , _num_channels(3)
    , _config(config)
{
    SCOPED_EVENT("Building texture", _name.c_str());
    Logger::log("Building texture '%s'", _name.c_str());

    verify_resolution(resolution, static_cast<int32_t>(rgb_data.size()));
    build_from_buffer(rgb_data.data());
}

Texture::Texture(
    const std::string& name,
    const std::vector<math::Vector4u8>& rgba_data,
    const math::Vector2i& resolution,
    const Config& config
)
    : _name(name)
    , _resolution(resolution)
    , _num_channels(4)
    , _config(config)
{
    SCOPED_EVENT("Building texture", _name.c_str());
    Logger::log("Building texture '%s'", _name.c_str());

    verify_resolution(resolution, static_cast<int32_t>(rgba_data.size()));
    build_from_buffer(rgba_data.data());
}

Texture::Texture(
    const std::string& name,
    int32_t num_channels,
    const math::Vector2i& resolution,
    const Config& config
)
    : _name(name)
    , _resolution(resolution)
    , _num_channels(num_channels)
    , _config(config)
{
    SCOPED_EVENT("Building texture", _name.c_str());
    Logger::log("Building texture '%s'", _name.c_str());

    build_from_buffer(nullptr);
}

Texture::~Texture()
{
    SCOPED_EVENT("Destroying texture", _name.c_str());
    Logger::log("Destroying texture '%s'", _name.c_str());

    glDeleteTextures(1, &_tex);
}

void Texture::bind(GLint slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, _tex);
}

void Texture::unbind(GLint slot) const
{
    // TODO: slot should be stored from bind and used automatically
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, 0);
}

const std::string& Texture::name() const noexcept
{
    return _name;
}

GLuint Texture::raw() const noexcept
{
    return _tex;
}

math::Vector2i Texture::resolution() const noexcept
{
    return _resolution;
}

void Texture::verify_resolution(const math::Vector2i& resolution, int32_t num_pixels) const
{
    if (resolution.area() != num_pixels)
    {
        throw std::runtime_error(strtools::catf(
            "Texture %s has a resolution of %dx%d (%dpx) but %dpx",
            _name.c_str(), resolution.x, resolution.y, resolution.magnitude_sqr(), num_pixels
        ));
    }
}

void Texture::build_from_buffer(const void* texture_data)
{
    glGenTextures(1, &_tex);
    glBindTexture(GL_TEXTURE_2D, _tex);
    glObjectLabel(GL_TEXTURE, _tex, -1, _name.c_str());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, _config.wrap_x);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, _config.wrap_y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _config.min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _config.max_filter);

    GLenum texture_format;
    switch (_num_channels)
    {
        case 3:
        {
            texture_format = GL_RGB;
            break;
        }
        case 4:
        {
            texture_format = GL_RGBA;
            break;
        }
        default:
        {
            throw std::runtime_error(strtools::catf("Cannot load texture with %d color channels", _num_channels));
        }
    }

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _resolution.x, _resolution.y, 0, texture_format, GL_UNSIGNED_BYTE, texture_data);

    if (_config.generate_mipmaps)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}
