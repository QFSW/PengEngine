#pragma once

#include <GL/glew.h>

#include <rendering/render_features.h>

namespace rendering
{
    void object_label(GLenum identifier, GLuint name, GLsizei length, const GLchar* label)
    {
        if (RenderFeatureSet::get().has_render_features(RenderFeatures::object_label))
        {
            glObjectLabel(identifier, name, length, label);
        }
    }
}