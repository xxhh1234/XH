#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"

namespace XH
{
	class XH_API Application 
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
	private:
		bool OnWindowClose(WindowCloseEvent& e);


		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	// To be defined in the client
	Application* CreateApplication();
}
