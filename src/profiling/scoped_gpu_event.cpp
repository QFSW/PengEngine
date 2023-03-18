#ifndef NO_PROFILING

#include "scoped_gpu_event.h"

#include <GL/glew.h>

using namespace profiling;

ScopedGPUEvent::ScopedGPUEvent(const char* name)
{
    glPushDebugGroup(GL_DEBUG_SOURCE_APPLICATION, 0, -1, name);
}

ScopedGPUEvent::~ScopedGPUEvent()
{
    glPopDebugGroup();
}

#endif
