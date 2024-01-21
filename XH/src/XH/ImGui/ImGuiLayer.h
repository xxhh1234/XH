#pragma once

#include "XH/Core/Layer.h"

#include "XH/Events/MouseEvent.h"
#include "XH/Events/KeyEvent.h"
#include "XH/Events/ApplicationEvent.h"

namespace XH
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        virtual ~ImGuiLayer() override = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;

        void Begin();
        void End();

    private:
        float m_Time = 0.0f;
    };
}