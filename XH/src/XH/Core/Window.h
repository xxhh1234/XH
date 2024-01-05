#pragma once

#include "xhpch.h"

#include "Core.h"
#include "XH/Events/Event.h"
#include "XH/Events/ApplicationEvent.h"
#include "XH/Events/KeyEvent.h"
#include "XH/Events/MouseEvent.h"

namespace XH
{
	struct WindowProps 
	{
		std::string Title;
		int Width;
		int Height;
		WindowProps(const std::string& title = "XH Engine",
							unsigned int width = 1280,
							unsigned int height = 720)
			:Title(title), Width(width), Height(height)
		{}
	};

	// Interface representing a desktop system based window
	class XH_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback)  = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
