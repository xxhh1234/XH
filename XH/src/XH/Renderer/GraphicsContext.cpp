#include "xhpch.h"
#include "GraphicsContext.h"

#include <glad/glad.h>
#include "RendererAPI.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace XH
{
    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        switch (Renderer::GetAPI())
        {
            case RendererAPI::API::None:    XH_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
            case RendererAPI::API::OpenGL:  return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
        }

        XH_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
