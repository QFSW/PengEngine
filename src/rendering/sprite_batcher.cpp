#include "sprite_batcher.h"

#include <ranges>
#include <execution>

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

    preprocess_draws(sprite_draws_in, _processed_draw_buffer);
    sort_draws(_processed_draw_buffer);
    bin_draws(_processed_draw_buffer, _draw_bin_buffer);
    emit_draws(_draw_bin_buffer, draws_out);
}

void SpriteBatcher::flush()
{
    // TODO: would be nicer if we could use the = nullptr syntax
    _sprite_mesh = {};
    _material_pools.clear();
    _buffer_pool.resources.clear();
}

SpriteBatcher::DrawBin::DrawBin()
    : depth_range(std::numeric_limits<float>::min(), std::numeric_limits<float>::min())
{ }

void SpriteBatcher::DrawBin::add_draw(const ProcessedSpriteDraw& processed_draw, const BinKey& bin_key)
{
    if (processed_draws.empty())
    {
        key = bin_key;
        depth_range = Vector2f(processed_draw.z_depth, processed_draw.z_depth);
    }
    else
    {
        depth_range.y = processed_draw.z_depth;
    }

    processed_draws.push_back(processed_draw);
}

bool SpriteBatcher::DrawBin::approx_flat() const noexcept
{
    return depth_range.y - depth_range.x < epsilon;
}

bool SpriteBatcher::DrawBin::approx_overlaps(float z_depth) const noexcept
{
    return z_depth - depth_range.x > -epsilon
        && z_depth - depth_range.y < +epsilon;
}

bool SpriteBatcher::DrawBin::empty() const noexcept
{
    return processed_draws.empty();
}

void SpriteBatcher::preprocess_draws(
    const std::vector<SpriteDrawCall>& sprite_draws_in,
    std::vector<ProcessedSpriteDraw>& processed_draws_out
) const
{
    SCOPED_EVENT("SpriteBatcher - preprocess draws");
    processed_draws_out.clear();
    processed_draws_out.reserve(sprite_draws_in.size());

    for (const SpriteDrawCall& sprite_draw : sprite_draws_in)
    {
        processed_draws_out.push_back(preprocess_draw(sprite_draw));
    }
}

void SpriteBatcher::sort_draws(std::vector<ProcessedSpriteDraw>& processed_draws_in_out) const
{
    SCOPED_EVENT("SpriteBatcher - sort draws");

    std::sort(std::execution::par_unseq, processed_draws_in_out.begin(), processed_draws_in_out.end(),
        [](const ProcessedSpriteDraw& x, const ProcessedSpriteDraw& y)
        {
            return x.z_depth < y.z_depth;
        });
}

void SpriteBatcher::bin_draws(
    const std::vector<ProcessedSpriteDraw>& processed_draws_in,
    std::vector<DrawBin>& draw_bins_out
) const
{
    SCOPED_EVENT("SpriteBatcher - bin draws");
    draw_bins_out.clear();

    // Translucent sprites require that bins are broken such that two separate translucent
    // bins never overlap in their z-depth range, but allow a small deviation for floating point errors
    //
    // To do this, we only allow 2 scenarios for incomplete alpha bins at any given time:
    // 1. A single bin of any depth range
    // 2. Multiple bins with approximately flat and equal depth
    std::vector<DrawBin> alpha_bins;
    std::unordered_map<BinKey, DrawBin> opaque_bins;

    for (const ProcessedSpriteDraw& processed_draw : processed_draws_in)
    {
        const bool requires_alpha =
            processed_draw.instance_data.color.w < 0.99f ||
            processed_draw.texture->has_alpha();

        const BinKey bin_key = std::make_tuple(processed_draw.texture, requires_alpha);

        // Opaque sprites can always be binned together if they have compatible textures
        if (!requires_alpha)
        {
            opaque_bins[bin_key].add_draw(processed_draw, bin_key);
            continue;
        }

        // Create an alpha bin if there aren't any
        if (alpha_bins.empty())
        {
            DrawBin& alpha_bin = alpha_bins.emplace_back();
            alpha_bin.add_draw(processed_draw, bin_key);
            continue;
        }

        // Consider the single bin case separately to multi bin as per (1)
        if (alpha_bins.size() == 1)
        {
            DrawBin& alpha_bin = alpha_bins[0];

            if (alpha_bin.key == bin_key)
            {
                // If it matches then allow it to grow in depth range
                alpha_bin.add_draw(processed_draw, bin_key);
                continue;
            }

            if (!alpha_bin.approx_flat())
            {
                // If it doesn't and the bin isn't flat, it needs flushing and create a new bin
                draw_bins_out.push_back(std::move(alpha_bin));
                alpha_bin = {};
                alpha_bin.add_draw(processed_draw, bin_key);
                continue;
            }
        }

        // If we have multiple bins now, they must all be flat and the new draw must overlap with all of them
        // Check that this is true, and if it is, add the draw to the corresponding bin
        // Otherwise flush all of them and create a new bin
        bool compatible = true;
        DrawBin* matching_bin = nullptr;

        for (DrawBin& flat_bin : alpha_bins)
        {
            compatible &= flat_bin.approx_overlaps(processed_draw.z_depth);
            if (flat_bin.key == bin_key)
            {
                matching_bin = &flat_bin;
            }
        }

        // Flush all flat bins since the new draw is incompatible
        if (!compatible)
        {
            draw_bins_out.append_range(std::move(alpha_bins));
            alpha_bins.clear();
            matching_bin = nullptr;
        }

        // Add to the existing bin if we have it
        if (matching_bin)
        {
            matching_bin->add_draw(processed_draw, bin_key);
            continue;
        }

        // Create a new bin otherwise
        DrawBin& alpha_bin = alpha_bins.emplace_back();
        alpha_bin.add_draw(processed_draw, bin_key);
    }

    draw_bins_out.append_range(std::move(alpha_bins));
    for (DrawBin& opaque_bin : opaque_bins | std::views::values)
    {
        draw_bins_out.push_back(std::move(opaque_bin));
    }
}

void SpriteBatcher::emit_draws(const std::vector<DrawBin>& draw_bins_in, std::vector<DrawCall>& draws_out)
{
    SCOPED_EVENT("SpriteBatcher - create draws");

    for (const DrawBin& draw_bin : draw_bins_in)
    {
        const bool requires_alpha = std::get<1>(draw_bin.key);
        if (draw_bin.processed_draws.size() == 1)
        {
            draws_out.push_back(
                emit_simple_draw(draw_bin.processed_draws[0], requires_alpha)
            );
        }
        else if (draw_bin.processed_draws.size() > 1)
        {
            draws_out.push_back(
                emit_instanced_draw(draw_bin.processed_draws, requires_alpha)
            );
        }
    }
}

DrawCall SpriteBatcher::emit_simple_draw(const ProcessedSpriteDraw& processed_draw, bool requires_alpha)
{
    const MaterialPoolKey pool_key = std::make_tuple(false, requires_alpha);
    peng::shared_ref<Material> material = get_pooled_material(pool_key);

    // TODO: use uniform caches
    material->set_parameter("color_tex", processed_draw.texture);
    material->set_parameter("base_color", processed_draw.instance_data.color);
    material->set_parameter("mvp_matrix", processed_draw.instance_data.mvp_matrix);
    material->set_parameter("tex_scale", processed_draw.instance_data.tex_scale);
    material->set_parameter("tex_offset", processed_draw.instance_data.tex_offset);

    const float order = material->shader()->requires_blending()
        ? -processed_draw.z_depth
        : +processed_draw.z_depth;

    return DrawCall{
        .mesh = get_sprite_mesh(),
        .material = material,
        .order = order,
        .instance_count = 1
    };
}

DrawCall SpriteBatcher::emit_instanced_draw(const std::vector<ProcessedSpriteDraw>& processed_draws, bool requires_alpha)
{
    const int32_t num_sprites = static_cast<int32_t>(processed_draws.size());
    check(num_sprites > 1);

    SCOPED_EVENT("SpriteBatcher - emit instanced draw", strtools::catf_temp("%d sprites", num_sprites));

    const MaterialPoolKey pool_key = std::make_tuple(true, requires_alpha);

    peng::shared_ref<Material> material = get_pooled_material(pool_key);
    peng::shared_ref<StructuredBuffer<SpriteInstanceData>> buffer = get_pooled_buffer();

    std::vector<SpriteInstanceData> instance_data;
    instance_data.reserve(processed_draws.size());

    float avg_depth = 0;
    auto consume_draw = [&](const ProcessedSpriteDraw& processed_draw)
    {
        avg_depth += processed_draw.z_depth;
        instance_data.push_back(processed_draw.instance_data);
    };

    const bool requires_blend = material->shader()->requires_blending();
    if (requires_blend)
    {
        // Translucent sprites need to be drawn in reverse z-depth order
        for (const ProcessedSpriteDraw& processed_draw : processed_draws | std::views::reverse)
        {
            consume_draw(processed_draw);
        }
    }
    else
    {
        for (const ProcessedSpriteDraw& processed_draw : processed_draws)
        {
            consume_draw(processed_draw);
        }
    }

    buffer->upload(instance_data);
    material->set_parameter("color_tex", processed_draws[0].texture);
    material->set_buffer("sprite_instance_data", buffer);

    avg_depth /= static_cast<float>(processed_draws.size());
    const float order = requires_blend
        ? -avg_depth
        : +avg_depth;

    return DrawCall{
        .mesh = get_sprite_mesh(),
        .material = material,
        .order = order,
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
        .z_depth = mvp_matrix.get_translation().z,
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
