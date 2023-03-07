#include "json_support.h"

using namespace math;

void math::to_json(nlohmann::json& j, const Transform& t)
{
    j = nlohmann::json({
        {"position", t.position},
        {"scale", t.scale},
        {"rotation", t.rotation}
    });
}

void math::from_json(const nlohmann::json& j, Transform& t)
{
    json_try_get_to(j, "position", t.position);
    json_try_get_to(j, "scale", t.scale);
    json_try_get_to(j, "rotation", t.rotation);
}