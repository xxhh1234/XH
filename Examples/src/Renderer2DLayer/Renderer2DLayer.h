#pragma once

#include <XH.h>
#include "imgui/imgui.h"
#include "ParticleSystem1.h"

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

    XH::Ref<XH::Texture2D> m_CheckerBoardTexture;

    ParticleSystem1 m_ParticleSystem;
    ParticleProps1 m_Particle;

    XH::Ref<XH::Texture2D> m_SpriteSheet;
    XH::Ref<XH::SubTexture2D> m_Sprite1, m_Sprite2, m_Sprite3;
};