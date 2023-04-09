#include "window_icon.h"

#include <stdexcept>

#include <core/asset_definition.h>
#include <core/logger.h>
#include <utils/strtools.h>
#include <libs/nlohmann/json.hpp>
#include <profiling/scoped_event.h>

#pragma warning( push, 0 )
#include <libs/stb/stb_image.h>
#pragma warning( pop )

#include "window_subsystem.h"

using namespace rendering;

WindowIcon::WindowIcon(const std::string& name, const std::string& texture_path)
    : _name(name)
{
    SCOPED_EVENT("Building window icon", _name.c_str());
    Logger::log("Building window icon '%s'", _name.c_str());
    Logger::log("Loading texture data '%s'", texture_path.c_str());

    _image.pixels = stbi_load(texture_path.c_str(), &_image.width, &_image.height, nullptr, 4);
    if (!_image.pixels)
    {
        throw std::runtime_error(strtools::catf("Could not load texture at %s", texture_path.c_str()));
    }
}

WindowIcon::~WindowIcon()
{
    SCOPED_EVENT("Destroying window icon", _name.c_str());
    Logger::log("Destroying window icon '%s'", _name.c_str());

    stbi_image_free(_image.pixels);
}

peng::shared_ref<WindowIcon> WindowIcon::load_asset(const AssetDefinition& asset_def)
{
    const std::string name = asset_def.json_def["name"].get<std::string>();
    const std::string texture_path = asset_def.json_def["texture"].get<std::string>();

    return peng::make_shared<WindowIcon>(name, texture_path);
}

void WindowIcon::use() const
{
    use(WindowSubsystem::get().window_handle());
}

void WindowIcon::use(GLFWwindow* window) const
{
    glfwSetWindowIcon(window, 1, &_image);
}

const std::string& WindowIcon::name() const noexcept
{
    return _name;
}

math::Vector2i WindowIcon::resolution() const noexcept
{
    return math::Vector2i(_image.width, _image.height);
}

void WindowIcon::verify_resolution(const math::Vector2i& resolution, int32_t num_pixels) const
{
    if (resolution.area() != num_pixels)
    {
        throw std::runtime_error(strtools::catf(
            "Texture %s has a resolution of %dx%d (%dpx) but %dpx",
            _name.c_str(), resolution.x, resolution.y, resolution.magnitude_sqr(), num_pixels
        ));
    }
}
