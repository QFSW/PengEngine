#ifndef NO_PROFILING

#include "scoped_gpu_event.h"

#include <GL/glew.h>

#include <rendering/render_features.h>

using namespace profiling;
using namespace rendering;

ScopedGPUEvent::ScopedGPUEvent(const char* name)
{
    if (RenderFeatureSet::get().has_render_features(RenderFeatures::debug_group))
    {
        glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name);
    }
}

ScopedGPUEvent::~ScopedGPUEvent()
{
    if (RenderFeatureSet::get().has_render_features(RenderFeatures::debug_group))
    {
        glPopDebugGroup();
    }
}

#endif
