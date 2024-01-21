#include "xhpch.h"
#include "VertexArray.h"

#include "RendererAPI.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace XH
{
    Ref<VertexArray> VertexArray::Create()
    {
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    XH_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
		}

		XH_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
    }
}
