#include "xhpch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "RendererCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace XH
{
    struct QuadVertex
    {
        glm::vec3 Position;
        glm::vec4 Color;
        glm::vec2 TexCoord;
        float     TexIndex;
        float     TilingFactor;
    };

    struct Renderer2DData
    {
        static const uint32_t MaxQuads = 20000;
        static const uint8_t  SingleQuadVertexCount = 4;
        static const uint8_t SingleQuadIndexCount = 6;
        static const uint32_t MaxVertices = MaxQuads * SingleQuadVertexCount;
        static const uint32_t MaxIndices = MaxQuads * SingleQuadIndexCount;
        static const uint32_t MaxTextureSlots = 32;

        Ref<VertexArray> QuadVertexArray;
        Ref<VertexBuffer> QuadVertexBuffer;
        Ref<Shader> QuadShader;


        QuadVertex* QuadVertexBufferBase = nullptr;
        QuadVertex* QuadVertexBufferPtr = nullptr;

        uint32_t QuadIndexCount;


        glm::vec4 QuadVertexPositions[SingleQuadVertexCount];

        std::array<Ref<Texture2D>, MaxTextureSlots> TextureSlots;
        uint8_t TextureSlotIndex;
    };

    static Renderer2DData s_Data;


    void Renderer2D::Init()
    {
        XH_PROFILE_FUNCTION();

        /*
        * --------------------------------------
        * -------------QUAD---------------------
        * --------------------------------------
        */
        s_Data.QuadVertexArray = VertexArray::Create();
        s_Data.QuadVertexPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[1] = { 0.5f, -0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[2] = { 0.5f,  0.5f, 0.0f, 1.0f };
        s_Data.QuadVertexPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };


        s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];
        s_Data.QuadVertexBuffer = VertexBuffer::Create(s_Data.MaxVertices * sizeof(QuadVertex));
        BufferLayout quadLayout =
        {
            {XH::ShaderDataType::Float3, "a_Position"},
            {XH::ShaderDataType::Float4, "a_Color"},
            {XH::ShaderDataType::Float2, "a_TexCoord"},
            {XH::ShaderDataType::Float,  "a_TexIndex"},
            {XH::ShaderDataType::Float,  "a_TilingFactor"}
        };
        s_Data.QuadVertexBuffer->SetLayout(quadLayout);
        s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);

        uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];
        uint32_t offset = 0;
        for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6, offset += 4)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;
        }
        Ref<XH::IndexBuffer>quadIB = IndexBuffer::Create(quadIndices, s_Data.MaxIndices);
        s_Data.QuadVertexArray->SetIndexBuffer(quadIB);
        delete[] quadIndices;

        s_Data.QuadShader = Shader::Create("assets/shaders/Texture.glsl");

        // Set WhiteTexture
        Ref<Texture2D> WhiteTexture = Texture2D::Create(TextureSpecification());
        uint32_t whiteTextureData = 0xffffffff;
        WhiteTexture->SetData(&whiteTextureData, sizeof(uint32_t));
        s_Data.TextureSlots[s_Data.TextureSlotIndex] = WhiteTexture;
    }

    void Renderer2D::Shutdown()
    {
        XH_PROFILE_FUNCTION();

        delete[] s_Data.QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera)
    {
        XH_PROFILE_FUNCTION();

        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.QuadShader->Bind();
        s_Data.QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
        s_Data.QuadShader->SetMat4("u_Transform", glm::mat4(1.0f));

        StartBatch();
    }

    void Renderer2D::EndScene()
    {
        XH_PROFILE_FUNCTION();

        Flush();
    }

    void Renderer2D::Flush()
    {
        XH_PROFILE_FUNCTION();

        if (s_Data.QuadIndexCount)
        {
            uint32_t quadVertexCount = s_Data.QuadVertexBufferPtr - s_Data.QuadVertexBufferBase;
            s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, quadVertexCount * sizeof(QuadVertex));

            // Bind textures
            for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
                s_Data.TextureSlots[i]->Bind(i);

            int32_t samplers[s_Data.MaxTextureSlots];
            for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++)
                samplers[i] = i;
            s_Data.QuadShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);
            s_Data.QuadShader->Bind();
            RendererCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);
        }
    }

    void Renderer2D::StartBatch()
    {
        XH_PROFILE_FUNCTION();

        s_Data.QuadIndexCount = 0;
        s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

        s_Data.TextureSlotIndex = 1;
    }

    void Renderer2D::NextBatch()
    {
        XH_PROFILE_FUNCTION();

        Flush();
        StartBatch();
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color,
        float rotation, float tilingFactor)
    {
        XH_PROFILE_FUNCTION();

        DrawQuad({ position.x, position.y, 0.0f }, size, color, rotation, tilingFactor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color,
        float rotation, float tilingFactor)
    {
        XH_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f }) 
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
        DrawQuad(transform, color, tilingFactor);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color,
        float tilingFactor)
    {
        XH_PROFILE_FUNCTION();

        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        float textureIndex = 0.0f;

        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            NextBatch();

        for (uint32_t i = 0; i < s_Data.SingleQuadVertexCount; ++i)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = color;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            ++s_Data.QuadVertexBufferPtr;
        }

        s_Data.QuadIndexCount += s_Data.SingleQuadIndexCount;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, 
        float rotation, float tilingFactor, const glm::vec4& tintColor)
    {
        XH_PROFILE_FUNCTION();

        DrawQuad(glm::vec3(position, 0.0f), size, texture, rotation, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, 
        float rotation, float tilingFactor, const glm::vec4& tintColor)
    {
        XH_PROFILE_FUNCTION();

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f })
            * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), { 0.0f, 0.0f, 1.0f });
        DrawQuad(transform, texture, tilingFactor, tintColor);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,
        float tilingFactor, const glm::vec4& tintColor)
    {
        XH_PROFILE_FUNCTION();

        constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };
        float textureIndex = 0.0f;

        if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices)
            NextBatch();

        for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
        {
            if (*s_Data.TextureSlots[i] == *texture)
            {
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        if (textureIndex == 0.0f)
        {
            textureIndex = static_cast<float>(s_Data.TextureSlotIndex);
            if (s_Data.TextureSlotIndex >= Renderer2DData::MaxTextureSlots)
                NextBatch();
            else
                s_Data.TextureSlots[s_Data.TextureSlotIndex++] = texture;
        }

        for (uint32_t i = 0; i < s_Data.SingleQuadVertexCount; ++i)
        {
            s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVertexPositions[i];
            s_Data.QuadVertexBufferPtr->Color = tintColor;
            s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
            s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
            s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
            ++s_Data.QuadVertexBufferPtr;
        }
        s_Data.QuadIndexCount += s_Data.SingleQuadIndexCount;
    }

}