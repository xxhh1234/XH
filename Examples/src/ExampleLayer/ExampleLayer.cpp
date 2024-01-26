#include "ExampleLayer.h"


ExampleLayer::ExampleLayer()
        : Layer("Example"), m_CameraController(1280.0f / 720.0f, true) {}

void ExampleLayer::OnAttach()
{
    float squareVertices[] =
    {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };
    uint32_t squareIndices[] = { 0, 1, 2, 2, 3, 0 };

    m_SquareVA = VertexArray::Create();
    Ref<VertexBuffer>m_SquareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
    BufferLayout squareLayout =
    {
        {ShaderDataType::Float3, "a_Position"},
        {ShaderDataType::Float2, "a_TexCoord"}
    };
    m_SquareVB->SetLayout(squareLayout);
    m_SquareVA->AddVertexBuffer(m_SquareVB);

    Ref<IndexBuffer>m_SquareIB = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
    m_SquareVA->SetIndexBuffer(m_SquareIB);

    auto m_FlatShader = m_ShaderLibrary.Load("Square", "assets/shaders/Square.glsl");
    auto textureShader = m_ShaderLibrary.Load("Texture", "assets/shaders/Texture.glsl");
    m_Texture = Texture2D::Create("assets/textures/Checkerboard.png");
    m_ChernoLogoTexture = Texture2D::Create("assets/textures/ChernoLogo.png");
}

void ExampleLayer::OnUpdate()
{
    // XH_TRACE("DeltaTime: {0}s, {1}ms",  Time::deltatime, Time::GetMilliseconds());
    m_CameraController.OnUpdate();

    RendererCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
    RendererCommand::Clear();

    auto m_FlatShader = m_ShaderLibrary.Get("Square");
    m_FlatShader->Bind();
    m_FlatShader->SetFloat3("u_Color", m_SquareColor);
    auto textureShader = m_ShaderLibrary.Get("Texture");
    textureShader->SetInt("u_Texture", 0);

    Renderer::BeginScene(m_CameraController.GetCamera());

    glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));
    for (int y = 0; y < 20; y++)
    {
        for (int x = 0; x < 20; x++)
        {
            glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
            glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
            Renderer::Submit(m_FlatShader, m_SquareVA, transform);
        }
    }

    m_Texture->Bind(0);
    Renderer::Submit(textureShader, m_SquareVA);
    m_ChernoLogoTexture->Bind(0);
    Renderer::Submit(textureShader, m_SquareVA);

    Renderer::EndScene();
}

void ExampleLayer::OnEvent(Event& e)
{
    m_CameraController.OnEvent(e);
}

void ExampleLayer::OnImGuiRender() 
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
    ImGui::End();
}