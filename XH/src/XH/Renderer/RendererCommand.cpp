#include "xhpch.h"
#include "RendererCommand.h"

#include "RendererAPI.h"

namespace XH
{
    Scope<RendererAPI> RendererCommand::s_RendererAPI = RendererAPI::Create();
}