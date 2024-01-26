#pragma once

#include <XH.h>
#include <imgui/imgui.h>
#include "Level.h"

class GameLayer : public XH::Layer
{
public:
    GameLayer();
    ~GameLayer() = default;
    
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnUpdate() override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent(XH::Event& e) override;
    bool OnMouseButtonPressed(XH::MouseButtonPressedEvent& e);
    bool OnWindowResize(XH::WindowResizeEvent& e);
private:
    void CreateCamera(uint32_t width, uint32_t height);

private:
    XH::Scope<XH::OrthographicCamera> m_Camera;
    Level m_Level;
    ImFont* m_Font{};
    float m_Time = 0.0f;
    bool m_Blink = false;
    enum class GameState
    {
        Play=0, MainMenu=1, GameOver=2
    };

    GameState m_State = GameState::MainMenu;
};