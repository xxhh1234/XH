#pragma once

#include <XH.h>
#include "imgui/imgui.h"

class Renderer2DLayer : public XH::Layer
{
public:
    Renderer2DLayer();
    virtual ~Renderer2DLayer() override = default;

    virtual void OnAttach() override;

    virtual void OnUpdate() override;
    virtual void OnEvent(XH::Event& e) override;
    virtual void OnImGuiRender() override;
private:
    XH::OrthographicCameraController m_CameraController;
    glm::vec4 m_QuadColor = { 0.2f, 0.3f, 0.8f, 1.0f};

    struct ProfileResult
    {
        const char* Name;
        float Time;
    };
    std::vector<ProfileResult> m_ProfileResults;
};