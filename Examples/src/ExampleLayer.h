#pragma once

#include <XH.h>

#include "imgui/imgui.h"
using namespace XH;


class ExampleLayer : public Layer
{
public:
    ExampleLayer();
    virtual ~ExampleLayer() override = default;

    virtual void OnAttach() override;
    virtual void OnUpdate() override;
    virtual void OnEvent(Event& e) override;
    virtual void OnImGuiRender() override;
private:
    OrthographicCameraController m_CameraController;
    ShaderLibrary m_ShaderLibrary;

    Ref<VertexArray> m_SquareVA;
    glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

    Ref<Texture2D> m_Texture, m_ChernoLogoTexture;
};
