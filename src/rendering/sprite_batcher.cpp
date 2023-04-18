#include "sprite_batcher.h"

#include <ranges>
#include <profiling/scoped_event.h>
#include <utils/strtools.h>

#include "sprite.h"
#include "texture.h"
#include "material.h"
#include "draw_call.h"
#include "primitives.h"
#include "sprite_draw_call.h"

using namespace rendering;
using namespace math;

void SpriteBatcher::convert_draws(
    const std::vector<SpriteDrawCall>& sprite_draws_in,
    std::vector<DrawCall>& draws_out
)
{
    SCOPED_EVENT("SpriteBatcher - convert draws", strtools::catf_temp("%d sprites", sprite_draws_in.size()));

    for (MaterialPool& pool : _material_pools | std::views::values)
    {
        pool.num_used = 0;
    }

    _buffer_pool.num_used = 0;

    std::vector<ProcessedSpriteDraw> processed_draws = preprocess_draws(sprite_draws_in);
    DrawBins binned_draws = bin_draws(std::move(processed_draws));

    emit_draws(std::move(binned_draws), draws_out);
}

void SpriteBatcher::flush()
{
    // TODO: would be nicer if we could use the = nullptr syntax
    _sprite_mesh = {};
    _material_pools.clear();
    _buffer_pool.resources.clear();
}

std::vector<SpriteBatcher::ProcessedSpriteDraw> SpriteBatcher::preprocess_draws(
    const std::vector<SpriteDrawCall>& sprite_draws
) const
{
    SCOPED_EVENT("SpriteBatcher - preprocess draws");
    std::vector<ProcessedSpriteDraw> processed_draws;
    processed_draws.reserve(sprite_draws.size());

    for (const SpriteDrawCall& sprite_draw : sprite_draws)
    {
        processed_draws.push_back(preprocess_draw(sprite_draw));
    }

    return processed_draws;
}

SpriteBatcher::DrawBins SpriteBatcher::bin_draws(std::vector<ProcessedSpriteDraw>&& processed_draws) const
{
    SCOPED_EVENT("SpriteBatcher - bin draws");
    DrawBins binned_draws;

    for (ProcessedSpriteDraw& processed_draw : processed_draws)
    {
        const bool requires_alpha =
            processed_draw.instance_data.color.w < 0.99f ||
            processed_draw.texture->has_alpha();

        const BinKey bin_key = std::make_tuple(processed_draw.texture, requires_alpha);
        binned_draws[bin_key].push_back(std::move(processed_draw));
    }

    return binned_draws;
}

void SpriteBatcher::emit_draws(DrawBins&& binned_draws, std::vector<DrawCall>& draws_out)
{
    SCOPED_EVENT("SpriteBatcher - create draws");

    for (auto& [bin_key, draws] : binned_draws)
    {
        const bool requires_alpha = std::get<1>(bin_key);
        if (draws.size() == 1)
        {
            draws_out.push_back(
                emit_simple_draw(std::move(draws[0]), requires_alpha)
            );
        }
        else if (draws.size() > 1)
        {
            draws_out.push_back(
                emit_instanced_draw(std::move(draws), requires_alpha)
            );
        }
    }
}

DrawCall SpriteBatcher::emit_simple_draw(ProcessedSpriteDraw&& processed_draw, bool requires_alpha)
{
    const MaterialPoolKey pool_key = std::make_tuple(false, requires_alpha);
    peng::shared_ref<Material> material = get_pooled_material(pool_key);

    // TODO: use uniform caches
    material->set_parameter("color_tex", processed_draw.texture);
    material->set_parameter("base_color", processed_draw.instance_data.color);
    material->set_parameter("mvp_matrix", processed_draw.instance_data.mvp_matrix);
    material->set_parameter("tex_scale", processed_draw.instance_data.tex_scale);
    material->set_parameter("tex_offset", processed_draw.instance_data.tex_offset);

    const float z_depth = processed_draw.instance_data.mvp_matrix.get_translation().z;
    const float order = material->shader()->requires_blending()
        ? -z_depth
        : +z_depth;

    return DrawCall{
        .mesh = get_sprite_mesh(),
        .material = std::move(material),
        .order = order,
        .instance_count = 1
    };
}

DrawCall SpriteBatcher::emit_instanced_draw(std::vector<ProcessedSpriteDraw>&& processed_draws, bool requires_alpha)
{
    const int32_t num_sprites = static_cast<int32_t>(processed_draws.size());
    check(num_sprites > 1);

    SCOPED_EVENT("SpriteBatcher - emit instanced draw", strtools::catf_temp("%d sprites", num_sprites));

    const MaterialPoolKey pool_key = std::make_tuple(true, requires_alpha);

    peng::shared_ref<Material> material = get_pooled_material(pool_key);
    peng::shared_ref<StructuredBuffer<SpriteInstanceData>> buffer = get_pooled_buffer();

    std::vector<SpriteInstanceData> instance_data;
    instance_data.reserve(processed_draws.size());

    // TODO: might need to sort instance data by z-depth
    // TODO: not sure how to order instanced draws relative to each other to avoid blending artifacts
    // TODO: can change binning so that each bin has a depth range and they never overlap
    for (const ProcessedSpriteDraw& processed_draw : processed_draws)
    {
        instance_data.push_back(processed_draw.instance_data);
    }

    buffer->upload(instance_data);
    material->set_parameter("color_tex", processed_draws[0].texture);
    material->set_buffer("sprite_instance_data", buffer);

    // TODO: work out order
    return DrawCall{
        .mesh = get_sprite_mesh(),
        .material = std::move(material),
        .order = 0,
        .instance_count = num_sprites
    };
}

SpriteBatcher::ProcessedSpriteDraw SpriteBatcher::preprocess_draw(const SpriteDrawCall& sprite_draw) const
{
    const peng::shared_ref<const Sprite>& sprite = sprite_draw.sprite;

    // Determine mvp matrix of sprite with sprite size accounted for
    const Vector3f sprite_scale = Vector3f(sprite->size(), 1);
    const Matrix4x4f sprite_matrix = Matrix4x4f::from_scale(sprite_scale);
    const Matrix4x4f mvp_matrix = sprite_draw.mvp_matrix * sprite_matrix;

    // Calculate texture coordinates of sprite
    // Texture coordinates have an inverted y compared to the pixel position
    Vector2i pos_corrected = sprite->position();
    pos_corrected.y = sprite->texture()->resolution().y - (pos_corrected.y + sprite->resolution().y);

    const Vector2f texture_res = Vector2f(sprite->texture()->resolution());
    const Vector2f tex_scale = Vector2f(sprite->resolution()) / texture_res;
    const Vector2f tex_offset = Vector2f(pos_corrected) / texture_res;

    return ProcessedSpriteDraw{
        .texture = sprite->texture(),
        .instance_data = SpriteInstanceData{
            .color = sprite_draw.color,
            .mvp_matrix = mvp_matrix,
            .tex_scale = tex_scale,
            .tex_offset = tex_offset
        }
    };
}

peng::shared_ref<const Mesh> SpriteBatcher::get_sprite_mesh()
{
    if (!_sprite_mesh)
    {
        _sprite_mesh = Primitives::quad();
    }

    return _sprite_mesh.to_shared_ref();
}

peng::shared_ref<Material> SpriteBatcher::get_pooled_material(const MaterialPoolKey& key)
{
    MaterialPool& pool = _material_pools[key];

    if (pool.num_used == pool.resources.size())
    {
        peng::shared_ref<Material> new_material = peng::make_shared<Material>(
            [key]
            {
                if (key == std::make_tuple(false, false))
                {
                    return Primitives::sprite_shader();
                }

                if (key == std::make_tuple(false, true))
                {
                    return Primitives::sprite_alpha_shader();
                }

                if (key == std::make_tuple(true, false))
                {
                    return Primitives::sprite_instanced_shader();
                }

                if (key == std::make_tuple(true, true))
                {
                    return Primitives::sprite_instanced_alpha_shader();
                }

                check(false);
                return Primitives::sprite_shader();
            }()
        );

        pool.resources.push_back(std::move(new_material));
    }

    return pool.resources[pool.num_used++];
}

peng::shared_ref<StructuredBuffer<SpriteBatcher::SpriteInstanceData>> SpriteBatcher::get_pooled_buffer()
{
    if (_buffer_pool.num_used == _buffer_pool.resources.size())
    {
        _buffer_pool.resources.push_back(
            peng::make_shared<StructuredBuffer<SpriteInstanceData>>(
                strtools::catf("SpriteBatcher[%d]", _buffer_pool.num_used),
                GL_DYNAMIC_DRAW
            )
        );
    }

    return _buffer_pool.resources[_buffer_pool.num_used++];
}
