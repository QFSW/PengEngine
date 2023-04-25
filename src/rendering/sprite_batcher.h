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
    // TODO: something in the rendering pipeline seems to have a frame of delay
    class SpriteBatcher
    {
    public:
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
            math::Matrix4x4f mvp_matrix;
            math::Vector2f tex_scale;
            math::Vector2f tex_offset;
        };

        struct ProcessedSpriteDraw
        {
            peng::shared_ref<const Texture> texture;
            float z_depth = 0;
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

        class DrawBin
        {
        public:
            DrawBin();

            void add_draw(const ProcessedSpriteDraw& processed_draw, const BinKey& bin_key);
            [[nodiscard]] const std::vector<SpriteInstanceData>& instance_data() const noexcept;

            [[nodiscard]] float avg_depth() const noexcept;
            [[nodiscard]] bool approx_flat() const noexcept;
            [[nodiscard]] bool approx_overlaps(float z_depth) const noexcept;
            [[nodiscard]] bool empty() const noexcept;

            BinKey key;

        private:
            math::Vector2f _depth_range;
            std::vector<SpriteInstanceData> _instance_data;

            static constexpr float epsilon = 0.00001f;
        };

        // Preprocess all of the sprite draw calls
        void preprocess_draws(
            const std::vector<SpriteDrawCall>& sprite_draws_in,
            std::vector<ProcessedSpriteDraw>& processed_draws_out
        ) const;

        // Sorts draws by their z-depth
        void sort_draws(
            std::vector<ProcessedSpriteDraw>& processed_draws_in_out
        ) const;

        // Bins processed draws by {texture, alpha}
        // This way all draws in a bin can be merged into one draw call
        // Mutates the input for performance reasons
        void bin_draws(
            const std::vector<ProcessedSpriteDraw>& processed_draws_in,
            std::vector<DrawBin>& draw_bins_out
        ) const;

        // Emits draw calls from the binned processed draws
        // Bins with more than one draw will result in a merged instanced draw
        void emit_draws(
            const std::vector<DrawBin>& draw_bins_in,
            std::vector<DrawCall>& draws_out
        );

        // Emits a draw call when no instancing is used
        [[nodiscard]] DrawCall emit_simple_draw(const DrawBin& draw_bin);

        // Emits an instanced draw call when to batch multiple sprites together
        [[nodiscard]] DrawCall emit_instanced_draw(const DrawBin& draw_bin);

        [[nodiscard]] ProcessedSpriteDraw preprocess_draw(const SpriteDrawCall& sprite_draw) const;
        [[nodiscard]] peng::shared_ref<const Mesh> get_sprite_mesh();
        [[nodiscard]] peng::shared_ref<Material> get_pooled_material(const MaterialPoolKey& key);
        [[nodiscard]] peng::shared_ref<StructuredBuffer<SpriteInstanceData>> get_pooled_buffer();

        peng::shared_ptr<const Mesh> _sprite_mesh;
        std::unordered_map<MaterialPoolKey, MaterialPool> _material_pools;
        ResourcePool<StructuredBuffer<SpriteInstanceData>> _buffer_pool;
        std::vector<ProcessedSpriteDraw> _processed_draw_buffer;
        std::vector<DrawBin> _draw_bin_buffer;
    };
}
