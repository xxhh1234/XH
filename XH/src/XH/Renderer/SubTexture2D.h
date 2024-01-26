#pragma once

#include "glm/glm.hpp"
#include "Texture.h"

namespace XH
{
    class SubTexture2D
    {
    public:
        SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);
        static Ref<SubTexture2D> CreateFromCoords(Ref<Texture2D> texture, const glm::vec2& coords, 
            const glm::vec2& cellSize, const glm::vec2& spriteSize={1.0f, 1.0f});

        const Ref<Texture2D> GetTexture() const { return m_Texture; }
        const glm::vec2* GetTexCoords() const { return m_TexCoords; }
    private:
        glm::vec2 m_TexCoords[4];
        Ref<Texture2D> m_Texture;
    };
}