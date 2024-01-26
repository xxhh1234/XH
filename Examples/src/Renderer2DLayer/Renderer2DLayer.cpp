#pragma once

#include "Renderer2DLayer.h"
#include <chrono>
#include "XH/Core/Timer.h"

Renderer2DLayer::Renderer2DLayer()
    :Layer("Renderer2D"), m_CameraController(1280.0f/720.0f, true)
{

}

void Renderer2DLayer::OnAttach()
{
    m_CheckerBoardTexture = XH::Texture2D::Create("assets/textures/Checkerboard.png");

    m_Particle.Position = { 0.0f, 0.0f };
    m_Particle.Velocity = { 0.0f, 0.0f }, m_Particle.VelocityVariation = { 3.0f, 1.0f };
    m_Particle.SizeBegin = 0.5f, m_Particle.SizeEnd = 0.0f, m_Particle.SizeVariation = 0.3f;
    m_Particle.ColorBegin = { 254.0f / 255.0f, 212.0f / 255.0f, 123.0f / 255.0f, 1.0f };
    m_Particle.ColorEnd = { 254.0f / 255.0f, 109.0f / 255.0f, 41.0f / 255.0f, 1.0f };
    m_Particle.LifeTime = 1.0f;

    m_SpriteSheet = XH::Texture2D::Create("assets/textures/kenney_rpg-base/Spritesheet/RPGpack_sheet_2X.png");
    m_Sprite1 = XH::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2.0f, 4.0f }, { 128.0f, 128.0f }, {2.0f, 3.0f});
    m_Sprite2 = XH::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 1.0f, 1.0f }, { 128.0f, 128.0f }, {1.0f, 2.0f});
    m_Sprite3 = XH::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2.0f, 1.0f }, { 128.0f, 128.0f }, {1.0f, 2.0f});
}

void Renderer2DLayer::OnUpdate()
{
    XH_PROFILE_FUNCTION();

    m_CameraController.OnUpdate();
    XH::Renderer2D::ResetStats();

    XH::RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    XH::RendererCommand::Clear();
   
#if 0
    XH::Renderer2D::BeginScene(m_CameraController.GetCamera());
    XH::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.2f }, { 20.0f, 20.0f }, m_CheckerBoardTexture, 0.0f, 10.0f);
    XH::Renderer2D::EndScene();

    XH::Renderer2D::BeginScene(m_CameraController.GetCamera());
    for (float y = -5.0f; y < 5.0f; y += 0.5f)
    {
        for (float x = -5.0f; x < 5.0f; x += 0.5f)
        {
            glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
            XH::Renderer2D::DrawQuad({ x, y, -0.1f}, { 0.45f, 0.45f }, color);
        }
    }
    XH::Renderer2D::EndScene();
    
    if (XH::Input::IsMouseButtonPressed(XH::Mouse::ButtonLeft))
    {
        auto [x, y] = XH::Input::GetMousePos();
        auto width = XH::Application::Get().GetWindow().GetWidth();
        auto height = XH::Application::Get().GetWindow().GetHeight();

        auto& bounds = m_CameraController.GetBounds();
        auto& cameraPos = m_CameraController.GetCamera().GetPosition();
        x = x / width * bounds.GetWidth() - 0.5f * bounds.GetWidth();
        y = 0.5f * bounds.GetHeight() - y / height * bounds.GetHeight();
        m_Particle.Position = { x + cameraPos.x, y + cameraPos.y };
        for (int i = 0; i < 50; ++i)
            m_ParticleSystem.Emit(m_Particle);
    }

    m_ParticleSystem.OnUpdate();
    m_ParticleSystem.OnRender(m_CameraController.GetCamera());
#endif
    XH::Renderer2D::BeginScene(m_CameraController.GetCamera());
    
    XH::Renderer2D::DrawQuad({  -1.0f, 0.0f, 0.5f }, { 2.0f, 3.0f }, m_Sprite1);
    XH::Renderer2D::DrawQuad({   0.0f, 0.0f, 0.5f }, { 1.0f, 2.0f }, m_Sprite2);
    XH::Renderer2D::DrawQuad({   1.0f, 0.0f, 0.5f }, { 1.0f, 2.0f }, m_Sprite3);

    XH::Renderer2D::EndScene();
}

void Renderer2DLayer::OnEvent(XH::Event& e)
{
    m_CameraController.OnEvent(e);
}

void Renderer2DLayer::OnImGuiRender()
{
    XH_PROFILE_FUNCTION();
    /*
    ImGui::Begin("Settings");
    
    auto stats = XH::Renderer2D::GetStats();
    ImGui::Text("Renderer2D Stats:");
    ImGui::Text("Draw Calls: %d", stats.DrawCalls);
    ImGui::Text("Quads: %d", stats.QuadCount);
    ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
    ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
    
    ImGui::End();
    */
    
}
