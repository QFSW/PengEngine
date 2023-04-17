#pragma once

#include <vector>
#include <variant>
#include <unordered_map>

#include <memory/shared_ptr.h>
#include <math/matrix4x4.h>
#include <utils/hash_helpers.h>

#include "structured_buffer.h"

namespace rendering
{
    struct DrawCall;
    struct SpriteDrawCall;

    class Mesh;
    class Material;
    class Texture;

    // Converts a set of sprite draw calls into regular draw calls
    // Where possible, batches sprites together into instanced draws
    class SpriteBatcher
    {
    public:
        SpriteBatcher();

        void convert_draws(
            const std::vector<SpriteDrawCall>& sprite_draws_in,
            std::vector<DrawCall>& draws_out
        );

        // Frees internal resources that may no longer be in use
        // Should be used sparingly to avoid thrashing
        void flush();

    private:
        // Instance data that can vary per sprite between draw
        struct SpriteInstanceData
        {
            math::Vector4f color;
            math::Matrix4x4f view_matrix;
            math::Matrix4x4f model_matrix;
            math::Vector2f tex_scale;
            math::Vector2f tex_offset;
        };

        struct ProcessedSpriteDraw
        {
            peng::shared_ref<const Texture> texture;
            SpriteInstanceData instance_data;
        };

        template <typename T>
        struct ResourcePool
        {
            std::vector<peng::shared_ref<T>> resources;
            size_t num_used = 0;
        };

        // Material pools are keyed by {instanced, requires_alpha}
        using MaterialPool = ResourcePool<Material>;
        using MaterialPoolKey = std::tuple<bool, bool>;

        using BinKey = std::tuple<peng::shared_ptr<const Texture>, bool>;
        using DrawBins = std::unordered_map<BinKey, std::vector<ProcessedSpriteDraw>>;

        // Preprocess all of the sprite draw calls
        [[nodiscard]] std::vector<ProcessedSpriteDraw> preprocess_draws(
            const std::vector<SpriteDrawCall>& sprite_draws
        ) const;

        // Bins processed draws by {texture, alpha}
        // This way all draws in a bin can be merged into one draw call
        DrawBins bin_draws(std::vector<ProcessedSpriteDraw>&& processed_draws) const;

        // Emits draw calls from the binned processed draws
        // Bins with more than one draw will result in a merged instanced draw
        void emit_draws(DrawBins&& binned_draws, std::vector<DrawCall>& draws_out);

        // Emits a draw call when no instancing is used
        [[nodiscard]] DrawCall emit_simple_draw(ProcessedSpriteDraw&& processed_draw, bool requires_alpha);

        // Emits an instanced draw call when to batch multiple sprites together
        [[nodiscard]] DrawCall emit_instanced_draw(
            std::vector<ProcessedSpriteDraw>&& processed_draws, bool requires_alpha
        );

        [[nodiscard]] ProcessedSpriteDraw preprocess_draw(const SpriteDrawCall& sprite_draw) const;
        [[nodiscard]] peng::shared_ref<const Mesh> get_sprite_mesh();
        [[nodiscard]] peng::shared_ref<Material> get_pooled_material(const MaterialPoolKey& key);
        [[nodiscard]] peng::shared_ref<StructuredBuffer<SpriteInstanceData>> get_pooled_buffer();

        size_t _buffers_used;

        peng::shared_ptr<const Mesh> _sprite_mesh;
        std::unordered_map<MaterialPoolKey, MaterialPool> _material_pools;
        std::vector<peng::shared_ref<StructuredBuffer<SpriteInstanceData>>> _buffer_pool;
    };
}
