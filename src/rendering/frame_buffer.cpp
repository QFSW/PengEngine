#include "frame_buffer.h"

#include <cassert>

#include <core/logger.h>
#include <profiling/scoped_event.h>

using namespace rendering;

FrameBuffer::FrameBuffer(const math::Vector2i& resolution)
	: _resolution(resolution)
{
	SCOPED_EVENT("Building frame buffer");
	Logger::log("Building frame buffer");

	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	glGenTextures(1, &_tex);
	glBindTexture(GL_TEXTURE_2D, _tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _resolution.x, _resolution.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

}

FrameBuffer::~FrameBuffer()
{
	SCOPED_EVENT("Destroying frame buffer");
	Logger::log("Destroying frame buffer");

	glDeleteFramebuffers(1, &_fbo);
	glDeleteTextures(1, &_tex);
}

void FrameBuffer::bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _tex, 0);

	assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void FrameBuffer::unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
