#include "texture_binding_cache.h"

#include "texture.h"

using namespace rendering;

GLenum TextureBindingCache::bind_texture(const peng::shared_ref<const Texture>& texture)
{
    // If it's already bound
    if (const auto it = _textures_to_slots.find(texture); it != _textures_to_slots.end())
    {
        return it->second;
    }

    // Bind texture if its not already bound
    // Prefer a free slot over a new slot
    int32_t slot;

    if (!_free_slots.empty())
    {
        slot = _free_slots.back();
        _free_slots.pop_back();
        _texture_slots[slot] = texture;
    }
    else
    {
        // Allocate a new slot otherwise
        slot = static_cast<int32_t>(_texture_slots.size());
        _texture_slots.emplace_back(texture);
    }

    // Bind texture
    texture->bind(slot);
    _textures_to_slots[texture] = slot;
    _slots_to_textures[slot] = texture;

    return slot;
}

void TextureBindingCache::unbind_texture(const peng::shared_ref<const Texture>&)
{
    // TODO: implement
}

void TextureBindingCache::unbind_texture(GLint)
{
    // TODO: implement
}

void TextureBindingCache::unbind_all()
{
    _textures_to_slots.clear();
    _slots_to_textures.clear();

    for (int32_t i = 0; i < _texture_slots.size(); i++)
    {
        if (_texture_slots[i])
        {
            _free_slots.push_back(i);
            _texture_slots[i]->unbind(i);
            _texture_slots[i] = nullptr;
        }
    }
}

