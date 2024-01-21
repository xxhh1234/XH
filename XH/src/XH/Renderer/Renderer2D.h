#pragma once

#include "OrthographicCamera.h"
#include "XH/Renderer/Texture.h"

namespace XH
{
    class Renderer2D
    {
    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();
        static void Flush();

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f),
            float rotation=0.0f, float tilingFactor=1.0f);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = glm::vec4(1.0f),
            float rotation=0.0f, float tilingFactor=1.0f);
        static void DrawQuad(const glm::mat4& transform,
            const glm::vec4& color = glm::vec4(1.0f), float tilingFactor=1.0f);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture,
            float rotation=0.0f, float tilingFactor=1.0f, const glm::vec4& tintColor=glm::vec4(1.0f));
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, 
            float rotation=0.0f, float tilingFactor = 1.0f,  const glm::vec4& tintColor = glm::vec4(1.0f));
        static void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,
            float tilingFactor = 1.0f, const glm::vec4& tintColor=glm::vec4(1.0f));
    private:
        static void StartBatch();
        static void NextBatch();
    };
}