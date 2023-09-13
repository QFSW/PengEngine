#pragma once

#include <optional>

#include <utils/singleton.h>
#include <utils/enum_flags.h>

namespace rendering
{
    enum RenderFeatures
    {
        buffer = 1 << 0,
        object_label = 1 << 1,
        debug_group = 1 << 2,

        none = 0,
    };

    ENUM_FLAGS(RenderFeatures)

    class RenderFeatureSet : public utils::Singleton<RenderFeatureSet>
    {
        using Singleton::Singleton;

    public:
        void set_render_features(RenderFeatures features);
        [[nodiscard]] RenderFeatures get_render_features();

        [[nodiscard]] bool has_render_features(RenderFeatures features);

    private:
        std::optional<RenderFeatures> _features;
    };
}