#pragma once

#include "XH/Core/Core.h"
#include "XH/Events/Event.h"


namespace XH
{
	class XH_API Layer
	{
	public:
		Layer(const std::string& debugName="Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	private:
		std::string m_DebugName;
	};
}