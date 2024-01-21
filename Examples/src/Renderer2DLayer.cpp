#pragma once

#include "Renderer2DLayer.h"
#include <chrono>
#include "XH/Core/Timer.h"

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) {\
                m_ProfileResults.push_back(profileResult); })

Renderer2DLayer::Renderer2DLayer()
    :Layer("Renderer2D"), m_CameraController(1280.0f/720.0f, true)
{

}

void Renderer2DLayer::OnAttach()
{
}

void Renderer2DLayer::OnUpdate()
{
    XH_PROFILE_FUNCTION();
    {
        XH_PROFILE_SCOPE("Renderer2D::OnUpdate");
        m_CameraController.OnUpdate();
    }

    {
        XH_PROFILE_SCOPE("Renderer Prepare");
        XH::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
        XH::RendererCommand::Clear();
    }

    {
        XH_PROFILE_SCOPE("Renderer Draw");
        XH::Renderer2D::BeginScene(m_CameraController.GetCamera());

        XH::Ref<XH::Texture2D> texture = XH::Texture2D::Create("assets/textures/Checkerboard.png");

        XH::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, {0.8f, 0.2f, 0.3f, 1.0f}, -45.0f);
        XH::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.5f }, { 0.2f, 0.3f, 0.8f, 1.0f });
        XH::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 5.0f, 5.0f }, texture, 0.0f, 10.0f);

        XH::Renderer2D::EndScene();
    }
}

void Renderer2DLayer::OnEvent(XH::Event& e)
{
    m_CameraController.OnEvent(e);
}

void Renderer2DLayer::OnImGuiRender()
{
    /*
    ImGui::Begin("Settings");
    
    ImGui::ColorEdit3("Quad Color", glm::value_ptr(m_QuadColor));
    
    static char label[50];
    for (auto& result:m_ProfileResults)
    {
        strcpy(label, "%.3fms ");
        strcat(label, result.Name);
        ImGui::Text(label, result.Time);
    }
    
    m_ProfileResults.clear();
    
    ImGui::End();
    */
    
}
