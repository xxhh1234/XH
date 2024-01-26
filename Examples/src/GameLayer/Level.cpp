#include "Level.h"

#include "Random.h"

static glm::vec4 HSVtoRGB(const glm::vec3& hsv)
{
    int H = static_cast<int>(hsv.x * 360.0f);
    double S = hsv.y;
    double V = hsv.z;

    double C = S * V;
    double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
    double m = V - C;
    double Rs = 0, Gs = 0, Bs = 0;

    if (H >= 0 && H < 60)
    {
        Rs = C;
        Gs = X;
        Bs = 0;
    }
    else if (H >= 60 && H < 120)
    {
        Rs = X;
        Gs = C;
        Bs = 0;
    }
    else if (H >= 120 && H < 180)
    {
        Rs = 0;
        Gs = C;
        Bs = X;
    }
    else if (H >= 180 && H < 240)
    {
        Rs = 0;
        Gs = X;
        Bs = C;
    }
    else if (H >= 240 && H < 300)
    {
        Rs = X;
        Gs = 0;
        Bs = C;
    }
    else
    {
        Rs = C;
        Gs = 0;
        Bs = X;
    }
    return { Rs + m, Gs + m, Bs + m, 1.0f };
}

static bool PointInTri(const glm::vec2& p, const glm::vec2& p0, const glm::vec2& p1, const glm::vec2& p2)
{
    float s1 = p2.x * p0.y - p0.x * p2.y + p.x * (p2.y - p0.y) + p.y * (p0.x - p2.x);
    float s2 = p0.x * p1.y - p1.x * p0.y + p.x * (p0.y - p1.y) + p.y * (p1.x - p0.x);

    if ((s1 < 0) != (s2 < 0))
        return false;
    float s = p1.x * p2.y - p2.x * p1.y + p0.x * (p1.y - p2.y) + p0.y * (p2.x - p1.x);
    return s < 0 ? (s1 <= 0 && s1 + s2 >= s) : (s1 >= 0 && s1 + s2 <= s);
}

void Level::Init()
{
    m_TriangleTexture = XH::Texture2D::Create("assets/textures/Triangle.png");
    m_Player.LoadAssets();
    m_Pillars.resize(5);
    for (int i = 0; i < 5; ++i)
        CreatePillar(i, i * 10.0f);
}

void Level::OnUpdate()
{
    m_Player.OnUpdate();

    if (CollsionTest())
    {
        XH_INFO("Level::OnUpdate:true");
        GameOver();
        return;
    }

    m_PillarHSV.x += 0.1f * XH::Time::deltatime;
    if (m_PillarHSV.x > 1.0f)
        m_PillarHSV.x = 0.0f;

    if (m_Player.GetPosition().x > m_PillarTarget)
    {
        XH_INFO("Level::OnUpdate m_PillarIndex = {0} ; m_PillarTarget = {1}; m_PillarTarget + 20.0f = {2}",
            m_PillarIndex, m_PillarTarget, m_PillarTarget + 20.0f);
        CreatePillar(m_PillarIndex, m_PillarTarget + 20.0f);
        m_PillarIndex = ++m_PillarIndex % m_Pillars.size();
        m_PillarTarget += 10.0f;
    }
}

void Level::OnRender()
{
    const auto& playerPos = m_Player.GetPosition();
    glm::vec4 color = HSVtoRGB(m_PillarHSV);
    XH::Renderer2D::DrawQuad({ playerPos.x, 0.0f, -0.8f }, { 50.0f, 50.0f }, { 0.3f, 0.3f, 0.3f, 1.0f });
    XH::Renderer2D::DrawQuad({ playerPos.x, 0.0f, 34.0f }, { 50.0f, 50.0f }, color);
    XH::Renderer2D::DrawQuad({ playerPos.x, 0.0f, -34.0f }, { 50.0f, 50.0f }, color);

    for (auto& pillar : m_Pillars)
    {
        XH::Renderer2D::DrawQuad(pillar.TopPosition, pillar.TopScale, m_TriangleTexture, 180.0f, 1.0f, color);
        XH::Renderer2D::DrawQuad(pillar.BottomPosition, pillar.BottomScale, m_TriangleTexture, 0.0f, 1.0f, color);
    }

    m_Player.OnRender();
}

void Level::OnImGuiRender()
{
    m_Player.OnImGuiRender();
}

void Level::Reset()
{
    m_GameOver = false;
    m_Player.Reset();

    m_PillarTarget = 30.0f;
    m_PillarIndex = 0;
    for (int i = 0; i < 5; ++i)
        CreatePillar(i, i * 10.0f);
}

void Level::CreatePillar(int index, float offset)
{
    XH_INFO("Level::CreatePillar index = {0}; offset = {1}", index, offset);

    Pillar& pillar = m_Pillars[index];
    pillar.TopPosition.x = offset;
    pillar.BottomPosition.x = offset;
    pillar.TopPosition.z = index * 0.1f - 0.5f;
    pillar.BottomPosition.z = index * 0.1f - 0.5f + 0.05f;

    float center = Random::Float() * 35.0f - 17.5f;
    float gap = 2.0f + Random::Float() * 5.0f;

    pillar.TopPosition.y = 10.0f - ((10.0f - center) * 0.2f) + gap * 0.5f;
    pillar.BottomPosition.y = -10.0f - ((-10.0f - center) * 0.2f) - gap * 0.5f;
}

bool Level::CollsionTest()
{
    
    if (glm::abs(m_Player.GetPosition().y) > 8.5f)
        return true;
        
    glm::vec4 playerVertices[4] =
    {
        {-0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f, -0.5f, 0.0f, 1.0f},
        { 0.5f,  0.5f, 0.0f, 1.0f},
        {-0.5f,  0.5f, 0.0f, 1.0f}
    };

    const auto& pos = m_Player.GetPosition();
    glm::vec4 playerTransformVerts[4];

    for (int i = 0; i < 4; ++i)
    {
        playerTransformVerts[i] = glm::translate(glm::mat4(1.0f), { pos.x, pos.y, 0.0f })
            * glm::rotate(glm::mat4(1.0f), m_Player.GetRotation(), { 0.0f, 0.0f, 1.0f })
            * glm::scale(glm::mat4(1.0f), { 1.0f, 1.3f, 1.0f }) * playerVertices[i];
    }

    glm::vec4 pillarVertices[3] =
    {
        {-0.5f + 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
        { 0.5f - 0.1f, -0.5f + 0.1f, 0.0f, 1.0f},
        { 0.0f + 0.0f,  0.5f - 0.1f, 0.0f, 1.0f}
    };

    for (auto& p : m_Pillars)
    {
        glm::vec2 tri[3];

        for (int i = 0; i < 3; ++i)
        {
            tri[i] = glm::translate(glm::mat4(1.0f), { p.TopPosition.x, p.TopPosition.y, 0.0f })
                * glm::rotate(glm::mat4(1.0f), 180.0f, { 0.0f, 0.0f, 1.0f })
                * glm::scale(glm::mat4(1.0f), { p.TopScale.x, p.TopScale.y, 1.0f }) * pillarVertices[i];
        }

        for (auto& vert : playerTransformVerts)
        {
            if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))
                return true;
        }

        for (int i = 0; i < 3; ++i)
        {
            tri[i] = glm::translate(glm::mat4(1.0f), { p.BottomPosition.x, p.BottomPosition.y, 0.0f })
                * glm::scale(glm::mat4(1.0f), { p.BottomScale.x, p.BottomScale.y, 1.0f }) * pillarVertices[i];
        }

        for (auto& vert : playerTransformVerts)
        {
            if (PointInTri({ vert.x, vert.y }, tri[0], tri[1], tri[2]))
                return true;
        }


    }

    return false;
}

void Level::GameOver()
{
    XH_INFO("Level::GameOver()");
    m_GameOver = true;
}
