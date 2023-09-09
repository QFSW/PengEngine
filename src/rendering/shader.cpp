#include "shader.h"

#include <unordered_set>
#include <filesystem>

#include <core/asset.h>
#include <utils/utils.h>
#include <utils/io.h>
#include <memory/gc.h>
#include <profiling/scoped_event.h>

#include "shader_compiler.h"
#include "shader_buffer.h"
#include "primitives.h"

using namespace rendering;
using namespace math;

Shader::Shader(
    const std::string& name,
    const std::string& vert_shader_path,
    const std::string& frag_shader_path
)
    : Shader(utils::copy(name), vert_shader_path, frag_shader_path)
{ }

Shader::Shader(
    std::string&& name,
    const std::string& vert_shader_path,
    const std::string& frag_shader_path
)
    : _name(std::move(name))
    , _broken(false)
    , _draw_order(0)
    , _blend_mode(BlendMode::opaque)
{
    SCOPED_EVENT("Building shader", _name.c_str());
    Logger::log("Building shader '%s'", _name.c_str());

    ShaderCompiler compiler;
    PreprocessedShader preprocessed_vert_shader = compiler.preprocess_shader(vert_shader_path, ShaderType::vertex);
    PreprocessedShader preprocessed_frag_shader = compiler.preprocess_shader(frag_shader_path, ShaderType::fragment);

    const GLuint vert_shader = compiler.compile_shader(preprocessed_vert_shader);
    const GLuint frag_shader = compiler.compile_shader(preprocessed_frag_shader);

    std::unordered_set<std::string> seen_symbols;

    for (ShaderSymbol& symbol : preprocessed_vert_shader.symbols)
    {
        if (!seen_symbols.contains(symbol.identifier))
        {
            seen_symbols.insert(symbol.identifier);
            _symbols.push_back(std::move(symbol));
        }
    }

    for (ShaderSymbol& symbol : preprocessed_frag_shader.symbols)
    {
        if (!seen_symbols.contains(symbol.identifier))
        {
            seen_symbols.insert(symbol.identifier);
            _symbols.push_back(std::move(symbol));
        }
    }

    _broken |= !validate_shader_compile(vert_shader);
    _broken |= !validate_shader_compile(frag_shader);

    {
        namespace fs = std::filesystem;
        glObjectLabel(GL_SHADER, vert_shader, -1, fs::path(vert_shader_path).filename().string().c_str());
        glObjectLabel(GL_SHADER, frag_shader, -1, fs::path(frag_shader_path).filename().string().c_str());
    }

    Logger::log("Linking shader program");
    _program = glCreateProgram();
    glAttachShader(_program, vert_shader);
    glAttachShader(_program, frag_shader);
    glLinkProgram(_program);
    _broken |= !validate_shader_link(_program);

    glObjectLabel(GL_PROGRAM, _program, -1, _name.c_str());

    glDeleteShader(vert_shader);
    glDeleteShader(frag_shader);

    if (!_broken)
    {
        Logger::log("Extracting uniform information");

        GLint num_uniforms;
        glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &num_uniforms);

        _uniforms.reserve(num_uniforms);
        for (GLint i = 0; i < num_uniforms; i++)
        {
            constexpr int32_t buf_size = 512;
            char name_buf[buf_size];
            GLint name_length;
            GLint size;
            GLenum type;

            glGetActiveUniform(_program, i, buf_size, &name_length, &size, &type, name_buf);
            const GLint location = glGetUniformLocation(_program, name_buf);

            // If a uniform has a location of -1 it's a non user uniform like the built in gl_ uniforms
            if (location != -1)
            {
                Uniform uniform;
                uniform.location = location;
                uniform.name = name_buf;
                uniform.type = type;
                uniform.default_value = read_uniform(uniform);

                _uniforms.push_back(std::move(uniform));
            }
        }
    }
}

Shader::~Shader()
{
    SCOPED_EVENT("Destroying shader", _name.c_str());
    Logger::log("Destroying shader '%s'", _name.c_str());

    glDeleteProgram(_program);
}

peng::shared_ref<Shader> Shader::load_asset(const Archive& archive)
{
    const std::string vert = archive.read<std::string>("vert");
    const std::string frag = archive.read<std::string>("frag");

    peng::shared_ref<Shader> shader = memory::GC::alloc<Shader>(archive.name, vert, frag);
    shader->draw_order() = archive.read_or("draw_order", 0);
    shader->blend_mode() = static_cast<BlendMode>(archive.read_or("blend_mode", 0));

    return shader;
}

peng::shared_ref<const Shader> Shader::fallback()
{
    static Asset<Shader> asset("resources/shaders/core/fallback.asset");
    peng::shared_ref<const Shader> shader = asset.load();

    check(!shader->broken());
    return shader;
}

void Shader::use() const
{
    check(!_broken);
    glUseProgram(_program);

    switch (_blend_mode)
    {
        case BlendMode::opaque:
        {
            glDisable(GL_BLEND);
            glBlendFunc(GL_ONE, GL_ZERO);
            break;
        }
        case BlendMode::alpha_blend:
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            break;
        }
        default:
        {
            throw std::runtime_error(strtools::catf("Invalid blend mode %d", _blend_mode));
        }
    }
}

void Shader::bind_buffer(GLint index, const peng::shared_ref<const IShaderBuffer>& buffer) const
{
    check(index >= 0);

    glShaderStorageBlockBinding(_program, index, index);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, buffer->get_ssbo());
}

int32_t& Shader::draw_order() noexcept
{
    return _draw_order;
}

BlendMode& Shader::blend_mode() noexcept
{
    return _blend_mode;
}

const std::string& Shader::name() const noexcept
{
    return _name;
}

bool Shader::broken() const noexcept
{
    return _broken;
}

bool Shader::requires_blending() const noexcept
{
    switch (_blend_mode)
    {
        case BlendMode::alpha_blend: return true;
        default:                     return false;
    }
}

int32_t Shader::draw_order() const noexcept
{
    return _draw_order;
}

BlendMode Shader::blend_mode() const noexcept
{
    return _blend_mode;
}

GLint Shader::get_uniform_location(const std::string& name) const
{
    for (const Uniform& uniform : _uniforms)
    {
        if (uniform.name == name)
        {
            return uniform.location;
        }
    }

    return -1;
}

GLint Shader::get_buffer_location(const std::string& name) const
{
    const GLuint index = glGetProgramResourceIndex(_program, GL_SHADER_STORAGE_BLOCK, name.c_str());
    return index == GL_INVALID_INDEX
        ? -1
        : index;
}

std::optional<std::string> Shader::get_symbol_value(const std::string& identifier) const noexcept
{
    for (const ShaderSymbol& symbol : _symbols)
    {
        if (symbol.identifier == identifier)
        {
            return symbol.value;
        }
    }

    return {};
}

bool Shader::has_symbol(const std::string& identifier) const noexcept
{
    for (const ShaderSymbol& symbol : _symbols)
    {
        if (symbol.identifier == identifier)
        {
            return true;
        }
    }

    return false;
}

const std::vector<Shader::Uniform>& Shader::uniforms() const noexcept
{
    return _uniforms;
}

const std::vector<ShaderSymbol>& Shader::symbols() const noexcept
{
    return _symbols;
}

bool Shader::validate_shader_compile(GLuint shader) const
{
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if constexpr (Logger::enabled())
    {
        if (!success)
        {
            GLint error_length;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &error_length);

            std::vector<GLchar> error_log(error_length);
            glGetShaderInfoLog(shader, error_length, nullptr, error_log.data());
            Logger::error(error_log.data());
        }
    }

    return success == GL_TRUE;
}

bool Shader::validate_shader_link(GLuint shader) const
{
    GLint success;
    glGetProgramiv(shader, GL_LINK_STATUS, &success);

    if constexpr (Logger::enabled())
    {
        if (!success)
        {
            GLint error_length;
            glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &error_length);

            std::vector<GLchar> error_log(error_length);
            glGetProgramInfoLog(shader, error_length, nullptr, error_log.data());
            Logger::error(error_log.data());
        }
    }

    return success == GL_TRUE;
}

std::optional<Shader::Parameter> Shader::read_uniform(const Uniform& uniform) const
{
    switch (uniform.type)
    {
        case GL_INT:
        {
            int32_t value;
            glGetUniformiv(_program, uniform.location, &value);

            return value;
        }
        case GL_UNSIGNED_INT:
        {
            uint32_t value;
            glGetUniformuiv(_program, uniform.location, &value);

            return value;
        }
        case GL_FLOAT:
        {
            float value;
            glGetUniformfv(_program, uniform.location, &value);

            return value;
        }
        case GL_DOUBLE:
        {
            double value;
            glGetUniformdv(_program, uniform.location, &value);

            return value;
        }
        case GL_INT_VEC2:
        {
            int32_t value[2];
            glGetUniformiv(_program, uniform.location, value);

            return Vector2i(value[0], value[1]);
        }
        case GL_UNSIGNED_INT_VEC2:
        {
            uint32_t value[2];
            glGetUniformuiv(_program, uniform.location, value);

            return Vector2u(value[0], value[1]);
        }
        case GL_FLOAT_VEC2:
        {
            float value[2];
            glGetUniformfv(_program, uniform.location, value);

            return Vector2f(value[0], value[1]);
        }
        case GL_DOUBLE_VEC2:
        {
            double value[2];
            glGetUniformdv(_program, uniform.location, value);

            return Vector2d(value[0], value[1]);
        }
        case GL_INT_VEC3:
        {
            int32_t value[3];
            glGetUniformiv(_program, uniform.location, value);

            return Vector3i(value[0], value[1], value[2]);
        }
        case GL_UNSIGNED_INT_VEC3:
        {
            uint32_t value[3];
            glGetUniformuiv(_program, uniform.location, value);

            return Vector3u(value[0], value[1], value[2]);
        }
        case GL_FLOAT_VEC3:
        {
            float value[3];
            glGetUniformfv(_program, uniform.location, value);

            return Vector3f(value[0], value[1], value[2]);
        }
        case GL_DOUBLE_VEC3:
        {
            double value[3];
            glGetUniformdv(_program, uniform.location, value);

            return Vector3d(value[0], value[1], value[2]);
        }
        case GL_INT_VEC4:
        {
            int32_t value[4];
            glGetUniformiv(_program, uniform.location, value);

            return Vector4i(value[0], value[1], value[2], value[3]);
        }
        case GL_UNSIGNED_INT_VEC4:
        {
            uint32_t value[4];
            glGetUniformuiv(_program, uniform.location, value);

            return Vector4u(value[0], value[1], value[2], value[3]);
        }
        case GL_FLOAT_VEC4:
        {
            float value[4];
            glGetUniformfv(_program, uniform.location, value);

            return Vector4f(value[0], value[1], value[2], value[3]);
        }
        case GL_DOUBLE_VEC4:
        {
            double value[4];
            glGetUniformdv(_program, uniform.location, value);

            return Vector4d(value[0], value[1], value[2], value[3]);
        }
        case GL_FLOAT_MAT3:
        {
            std::array<float, 9> value;
            glGetUniformfv(_program, uniform.location, value.data());

            return Matrix3x3f(value);
        }
        case GL_DOUBLE_MAT3:
        {
            std::array<double, 9> value;
            glGetUniformdv(_program, uniform.location, value.data());

            return Matrix3x3d(value);
        }
        case GL_FLOAT_MAT4:
        {
            std::array<float, 16> value;
            glGetUniformfv(_program, uniform.location, value.data());

            return Matrix4x4f(value);
        }
        case GL_DOUBLE_MAT4:
        {
            std::array<double, 16> value;
            glGetUniformdv(_program, uniform.location, value.data());

            return Matrix4x4d(value);
        }
        case GL_SAMPLER_2D:
        {
            return Primitives::white_tex();
        }
        default:
        {
            Logger::get().logf(LogSeverity::error,
                "Unable to get default value of parameter %s due to unsupported uniform type 0x%x",
                uniform.name.c_str(), uniform.type
            );

            return std::nullopt;
        }
    }
}
