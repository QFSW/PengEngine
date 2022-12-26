#include "texture.h"

#include <stdexcept>

#include <core/logger.h>
#include <utils/strtools.h>
#include <common/common.h>

#pragma warning( push, 0 )
#define STB_IMAGE_IMPLEMENTATION
#include <libs/stb/stb_image.h>
#pragma warning( pop )

using namespace rendering;

Texture::Texture(const std::string& name, const std::string& texture_path)
	: _name(name)
{
	Logger::get().logf(LogVerbosity::Log, "Building texture '%s'", _name.c_str());
	Logger::get().logf(LogVerbosity::Log, "Loading texture data '%s'", texture_path.c_str());
	stbi_set_flip_vertically_on_load(true);
	stbi_uc* texture_data = stbi_load(texture_path.c_str(), &_resolution.x, &_resolution.y, &_num_channels, 0);
	if (!texture_data)
	{
		throw std::runtime_error(strtools::catf("Could not load texture at %s", texture_path.c_str()));
	}

	glGenTextures(1, &_tex);
	glBindTexture(GL_TEXTURE_2D, _tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(texture_data);
}

Texture::~Texture()
{
	Logger::get().logf(LogVerbosity::Log, "Destroying texture '%s'", _name.c_str());
	glDeleteTextures(1, &_tex);
}

void Texture::use(GLenum slot) const
{
	glActiveTexture(slot);
	glBindTexture(GL_TEXTURE_2D, _tex);
}
