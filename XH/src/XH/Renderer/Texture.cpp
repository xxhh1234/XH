#include "xhpch.h"
#include "Texture.h"

#include <glad/glad.h>
#include "RendererAPI.h"
#include "Renderer.h"
#include "Platform/OpenGL/OpenGLTexture.h"

namespace XH
{
	Ref<Texture2D> Texture2D::Create(const TextureSpecification& specification)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    XH_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(specification);
		}

		XH_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}

	Ref<Texture2D> Texture2D::Create(const std::string& path)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:    XH_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
			case RendererAPI::API::OpenGL:  return CreateRef<OpenGLTexture2D>(path);
		}

		XH_CORE_ASSERT(false, "Unknown RendererAPI!");
		return nullptr;
	}
}