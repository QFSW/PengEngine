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
		Texture(const std::string& name, const std::string& texture_path);

		Texture(
			const std::string& name,
			const std::vector<math::Vector3u8>& rgb_data,
			const math::Vector2i& resolution
		);

		Texture(
			const std::string& name,
			const std::vector<math::Vector4u8>& rgba_data,
			const math::Vector2i& resolution
		);

		// Creates an uninitialized texture with the specified resolution and channels
		Texture(
			const std::string& name,
			int32_t num_channels,
			const math::Vector2i& resolution
		);

		Texture(const Texture&) = delete;
		Texture(Texture&&) = delete;
		~Texture();

		void bind(GLenum slot) const;

		[[nodiscard]] GLuint raw() const noexcept;

	private:
		void verify_resolution(const math::Vector2i& resolution, int32_t num_pixels) const;
		void build_from_buffer(const void* texture_data);

		std::string _name;
		GLuint _tex;

		math::Vector2i _resolution;
		int32_t _num_channels;
	};
}
