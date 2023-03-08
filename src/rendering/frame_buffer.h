#pragma once

#include <GL/glew.h>

#include <memory/shared_ref.h>
#include <math/vector2.h>

namespace rendering
{
	class Texture;

	class FrameBuffer
	{
	public:
		FrameBuffer(const math::Vector2i& resolution);
		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&&) = delete;
		~FrameBuffer();

		void bind() const;
		void unbind() const;

	private:
		GLuint _fbo;

		math::Vector2i _resolution;
		peng::shared_ref<Texture> _texture;
	};
}