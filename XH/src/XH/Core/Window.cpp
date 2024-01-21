#include "xhpch.h"
#include "Window.h"

#include <Platform/Windows/WindowsWindow.h>

namespace XH
{
    Scope<Window> Window::Create(const WindowProps& props)
    {
#ifdef XH_PLATFORM_WINDOWS
		return CreateScope<WindowsWindow>(props);
#else
		XH_CORE_ASSERT(false, "Unknown platform!");
		return nullptr;
#endif
    }
}