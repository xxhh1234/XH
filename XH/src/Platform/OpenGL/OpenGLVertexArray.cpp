#include "xhpch.h"
#include "OpenGLVertexArray.h"

#include <glad/glad.h>

namespace XH
{
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Float2:   return GL_FLOAT;
            case ShaderDataType::Float3:   return GL_FLOAT;
            case ShaderDataType::Float4:   return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
        }

        XH_CORE_ASSERT(false, "Unknown ShaderDataType!");
        return 0;
    }

    OpenGLVertexArray::OpenGLVertexArray()
    {
        XH_PROFILE_FUNCTION();

        glCreateVertexArrays(1, &m_RendererID);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        XH_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &m_RendererID);
    }

    void OpenGLVertexArray::Bind() const
    {
        XH_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
    }
    
    void OpenGLVertexArray::UnBind() const
    {
        XH_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }
    
    void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
    {
        XH_PROFILE_FUNCTION();

        XH_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");

        glBindVertexArray(m_RendererID);
        vertexBuffer->Bind();

        const auto& layout = vertexBuffer->GetLayout();
        auto BindVertexBuffer = [&](BufferElement element, uint32_t offset, void (*fn)(uint32_t index, uint32_t divisor))
        {
            glEnableVertexAttribArray(m_VertexBufferIndex);
            glVertexAttribPointer(m_VertexBufferIndex,
                element.GetComponentCount(),
                ShaderDataTypeToOpenGLBaseType(element.Type),
                element.Normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                reinterpret_cast<const void*>(element.Offset + offset));
            if(fn)
                fn(m_VertexBufferIndex, 1);
            ++m_VertexBufferIndex;
        };
        for (const auto& element : layout)
        {
            switch (element.Type)
            {
                case ShaderDataType::Float:
                {
                    BindVertexBuffer(element, 0, nullptr);
                    break;
                }
                case ShaderDataType::Float2:
                {
                    BindVertexBuffer(element, 0, nullptr);
                    break;
                }
                case ShaderDataType::Float3:
                {
                    BindVertexBuffer(element, 0, nullptr);
                    break;
                }
                case ShaderDataType::Float4:
                {
                    BindVertexBuffer(element, 0, nullptr);
                    break;
                }
                case ShaderDataType::Int:
                {
                    BindVertexBuffer(element, 0, nullptr);
                    break;
                }
                case ShaderDataType::Int2:
                {
                    BindVertexBuffer(element, 0, nullptr);
                    break;
                }
                case ShaderDataType::Int3:
                {
                    BindVertexBuffer(element, 0, nullptr);
                    break;
                }
                case ShaderDataType::Int4:
                {
                    BindVertexBuffer(element, 0, nullptr);
                    break;
                }
                case ShaderDataType::Bool:
                {
                    BindVertexBuffer(element, 0, nullptr);
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4:
                {
                    uint8_t count = element.GetComponentCount();
                    for (uint8_t i = 0; i < count; i++)
                    {
                        BindVertexBuffer(element, sizeof(float) * count * i, glVertexAttribDivisor);
                    }
                    break;
                }
                default:
                    XH_CORE_ASSERT(false, "Unknown ShaderDataType!");
            }
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
    {
        XH_PROFILE_FUNCTION();

        glBindVertexArray(m_RendererID);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}