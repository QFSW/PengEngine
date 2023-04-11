#pragma once

#include <string>

#include <GLFW/glfw3.h>
#include <memory/shared_ref.h>
#include <math/vector2.h>

struct Archive;

namespace rendering
{
    class WindowIcon
    {
    public:
        WindowIcon(const std::string& name, const std::string& texture_path);
        WindowIcon(const WindowIcon&) = delete;
        WindowIcon(WindowIcon&&) = delete;
        ~WindowIcon();

        static peng::shared_ref<WindowIcon> load_asset(const Archive& archive);

        void use() const;
        void use(GLFWwindow* window) const;

        [[nodiscard]] const std::string& name() const noexcept;
        [[nodiscard]] math::Vector2i resolution() const noexcept;

    private:
        void verify_resolution(const math::Vector2i& resolution, int32_t num_pixels) const;

        std::string _name;
        GLFWimage _image;
    };
}
