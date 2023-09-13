#include "render_features.h"

#include <utils/check.h>

using namespace rendering;

void RenderFeatureSet::set_render_features(RenderFeatures features)
{
    _features = features;
}

RenderFeatures RenderFeatureSet::get_render_features()
{
    check(_features);
    return *_features;
}

bool RenderFeatureSet::has_render_features(RenderFeatures features)
{
    return (get_render_features() & features) == features;
}