#pragma once

#include <unordered_map>

#include <GL/glew.h>
#include <memory/shared_ptr.h>
#include <utils/singleton.h>

namespace rendering
{
    class Texture;

    // Cache for binding textures into GPU memory
    // TODO: this non-ideally extends the lifetime of textures automatically
    class TextureBindingCache : public utils::Singleton<TextureBindingCache>
    {
        using Singleton::Singleton;

    public:
        // Binds the provided texture to the cache and returns the binding slot
        // Will return an existing slot if the texture is already bound
        GLenum bind_texture(const peng::shared_ref<const Texture>& texture);

        void unbind_texture(const peng::shared_ref<const Texture>& texture);
        void unbind_texture(GLint slot);
        
        void unbind_all();

    private:
        std::vector<int32_t> _free_slots;
        std::vector<peng::shared_ptr<const Texture>> _texture_slots;
        std::unordered_map<peng::shared_ptr<const Texture>, GLint> _textures_to_slots;
        std::unordered_map<GLint, peng::shared_ptr<const Texture>> _slots_to_textures;
    };
}