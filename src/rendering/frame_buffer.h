#pragma once

#include <GL/glew.h>

#include <math/vector2.h>

namespace rendering
{
	// TODO: refactor out all of the render texture stuff so that we have a RenderTexture object similar to Texture
	//		 that can be bound to things
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
		GLuint _tex;

		math::Vector2i _resolution;
	};
}