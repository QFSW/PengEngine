#pragma once

#include <vector>
#include <string>

#include <profiling/scoped_event.h>
#include <utils/check.h>

#include "shader_buffer.h"
#include "gl_wrapper.h"

namespace rendering
{
    template <typename T>
    class StructuredBuffer : public IShaderBuffer
    {
    public:
        StructuredBuffer(const std::string& name, GLenum usage);
        ~StructuredBuffer() override;

        StructuredBuffer(const StructuredBuffer&) = delete;
        StructuredBuffer(StructuredBuffer&&) = delete;
        StructuredBuffer& operator=(const StructuredBuffer&) = delete;
        StructuredBuffer& operator=(StructuredBuffer&&) = delete;

        void upload(const std::vector<T>& data);

        [[nodiscard]] GLuint get_ssbo() const override;

    private:
        void release_ssbo();

        std::string _name;
        GLenum _usage;

        GLuint _ssbo;
        size_t _capacity;
        size_t _size;
    };

    template <typename T>
    StructuredBuffer<T>::StructuredBuffer(const std::string& name, GLenum usage)
        : _name(name)
        , _usage(usage)
        , _ssbo(0)
        , _capacity(0)
        , _size(0)
    { }

    template <typename T>
    StructuredBuffer<T>::~StructuredBuffer()
    {
        release_ssbo();
    }

    template <typename T>
    void StructuredBuffer<T>::upload(const std::vector<T>& data)
    {
        SCOPED_EVENT("StructuredBuffer - upload", _name.c_str());

        if (data.size() <= _capacity)
        {
            // If the data fits in our existing buffer then just upload new data
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
            glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, data.size() * sizeof(T), data.data());
            _size = data.size();
        }
        else
        {
            // Otherwise, free the old buffer and get a new one big enough for the data
            // TODO: make the growth policy less prone to thrashing
            release_ssbo();

            {
                SCOPED_EVENT("StructuredBuffer - allocate", _name.c_str());
                glGenBuffers(1, &_ssbo);
                glBindBuffer(GL_SHADER_STORAGE_BUFFER, _ssbo);
                glObjectLabelWrapped(GL_BUFFER, _ssbo, -1, _name.c_str());
                glBufferData(GL_SHADER_STORAGE_BUFFER, data.size() * sizeof(T), data.data(), _usage);
            }

            _size = data.size();
            _capacity = _size;
        }
    }

    template <typename T>
    GLuint StructuredBuffer<T>::get_ssbo() const
    {
        check(_ssbo);
        return _ssbo;
    }

    template <typename T>
    void StructuredBuffer<T>::release_ssbo()
    {
        if (_ssbo)
        {
            SCOPED_EVENT("StructuredBuffer - release", _name.c_str());

            glDeleteBuffers(1, &_ssbo);
            _ssbo = 0;
            _capacity = 0;
            _size = 0;
        }
    }
}
