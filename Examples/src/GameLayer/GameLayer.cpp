#include "GameLayer.h"

#include "Random.h"

GameLayer::GameLayer()
    :Layer("GameLayer")
{
    auto& window = XH::Application::Get().GetWindow();
    CreateCamera(window.GetWidth(), window.GetHeight());

    Random::Init();
}

void GameLayer::OnAttach()
{
    m_Level.Init();

    ImGuiIO io = ImGui::GetIO();
    m_Font = io.Fonts->AddFontFromFileTTF("assets\\fonts\\opensans\\OpenSans-Regular.ttf", 120.0f);
}

void GameLayer::OnDetach()
{

}

void GameLayer::OnUpdate()
{
    m_Time += XH::Time::deltatime;

    if (static_cast<int>(m_Time * 10.0f) % 8 > 4)
        m_Blink = !m_Blink;

    if (m_Level.IsGameOver())
    {
        XH_INFO("GameLayer::OnUpdate IsGameOver: true");
        m_State = GameState::GameOver;
    }

    const auto& playerPos = m_Level.GetPlayer().GetPosition();
    m_Camera->SetPosition({ playerPos.x, playerPos.y, 0.0f });
    
    if (m_State == GameState::Play)
        m_Level.OnUpdate();
    
    XH::RendererCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
    XH::RendererCommand::Clear();
    XH::Renderer2D::BeginScene(*m_Camera);
    m_Level.OnRender();
    XH::Renderer2D::EndScene();
}

void GameLayer::OnImGuiRender()
{
    switch(m_State)
    {
        case GameLayer::GameState::Play:
        {
            XH_INFO("GameLayer::OnImGuiRender Play");
            uint32_t playerScore = m_Level.GetPlayer().GetScore();
            std::string scoreStr = std::string("Score: " + std::to_string(playerScore));
            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
            break;
        }
        case GameLayer::GameState::MainMenu:
        {
            XH_INFO("GameLayer::OnImGuiRender MainMenu");
            auto pos = ImGui::GetWindowPos();
            auto width = XH::Application::Get().GetWindow().GetWidth();

            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (m_Blink)
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");
            break;
        }
        case GameLayer::GameState::GameOver:
        {
            XH_INFO("GameLayer::OnImGuiRender GameOver");
            auto pos = ImGui::GetWindowPos();
            auto width = XH::Application::Get().GetWindow().GetWidth();

            pos.x += width * 0.5f - 300.0f;
            pos.y += 50.0f;
            if (m_Blink)
                ImGui::GetForegroundDrawList()->AddText(m_Font, 120.0f, pos, 0xffffffff, "Click to Play!");

            pos.x += 200.0f;
            pos.y += 150.0f;
            uint32_t playerScore = m_Level.GetPlayer().GetScore();
            std::string scoreStr = std::string("Score: " + std::to_string(playerScore));

            ImGui::GetForegroundDrawList()->AddText(m_Font, 48.0f, ImGui::GetWindowPos(), 0xffffffff, scoreStr.c_str());
            break;
        }
        default:
            break;
    }
}

void GameLayer::OnEvent(XH::Event& e)
{
    XH::EventDispatcher dispatcher(e);
    dispatcher.Dispatch<XH::WindowResizeEvent>(XH_BIND_EVENT_FN(GameLayer::OnWindowResize));
    dispatcher.Dispatch<XH::MouseButtonPressedEvent>(XH_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
}

bool GameLayer::OnMouseButtonPressed(XH::MouseButtonPressedEvent& e)
{
    if (m_State == GameState::GameOver)
        m_Level.Reset();

    m_State = GameState::Play;
    return false;
}

bool GameLayer::OnWindowResize(XH::WindowResizeEvent& e)
{
    CreateCamera(e.GetWidth(), e.GetHeight());
    return false;
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);

    float camWidth = 8.0f;
    float bottom = -camWidth;
    float top = camWidth;
    float left = bottom * aspectRatio;
    float right = top * aspectRatio;
    m_Camera = XH::CreateScope<XH::OrthographicCamera>(left, right, bottom, top);
}
