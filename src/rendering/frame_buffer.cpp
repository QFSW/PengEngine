#include "frame_buffer.h"

#include <core/logger.h>
#include <profiling/scoped_event.h>

#include "texture.h"

using namespace rendering;

FrameBuffer::FrameBuffer(const std::string& name, const math::Vector2i& resolution)
	: _name(name)
	, _resolution(resolution)
{
	SCOPED_EVENT("Building framebuffer", _name.c_str());
	Logger::log("Building framebuffer '%s'", _name.c_str());

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
}

FrameBuffer::~FrameBuffer()
{
	SCOPED_EVENT("Destroying framebuffer", _name.c_str());
	Logger::log("Destroying framebuffer '%s'", _name.c_str());

	// TODO: if framebuffer is currently bound we should unbind before destroying it

	glDeleteFramebuffers(1, &_fbo);
}

void FrameBuffer::add_color_attachment()
{
	if (_color_attachments.size() >= GL_MAX_COLOR_ATTACHMENTS)
	{
		throw std::runtime_error(strtools::catf(
			"Cannot bind more than %d color attachments to a framebuffer", GL_MAX_COLOR_ATTACHMENTS
		));
	}

	std::string texture_name = strtools::catf("%s.ColorAttachment[%d]", _name.c_str(), _color_attachments.size());
	_color_attachments.push_back(peng::make_shared<Texture>(std::move(texture_name), 3, _resolution));
}

void FrameBuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	GLuint texture_count = 0;
	for (const peng::shared_ref<Texture>& texture : _color_attachments)
	{
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + texture_count, GL_TEXTURE_2D, texture->raw(), 0);
		texture_count++;
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		throw std::runtime_error(strtools::catf(
			"Cannot bind framebuffer '%s' as it is not framebuffer complete", _name.c_str()
		));
	}
}

void FrameBuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

const std::vector<peng::shared_ref<Texture>>& FrameBuffer::color_attachments() const noexcept
{
	return _color_attachments;
}
