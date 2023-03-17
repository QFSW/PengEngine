#pragma once

#include <vector>
#include <GL/glew.h>

#include <memory/shared_ref.h>
#include <math/vector2.h>

namespace rendering
{
	class Texture;

	// TODO: add object name for RenderDoc
	class FrameBuffer
	{
	public:
		FrameBuffer(const std::string& name, const math::Vector2i& resolution);
		FrameBuffer(const FrameBuffer&) = delete;
		FrameBuffer(FrameBuffer&&) = delete;
		~FrameBuffer();

		void add_color_attachment();

		void bind() const;
		void unbind() const;

		[[nodiscard]] const std::vector<peng::shared_ref<Texture>>& color_attachments() const noexcept;

	private:
		GLuint _fbo;

		std::string _name;
		math::Vector2i _resolution;
		std::vector<peng::shared_ref<Texture>> _color_attachments;
	};
}