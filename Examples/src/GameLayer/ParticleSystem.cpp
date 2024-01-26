#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
{
    m_ParticlePool.resize(1000);
}

void ParticleSystem::Emit(const ParticleProps& particleProps)
{
    Particle& particle = m_ParticlePool[m_PoolIndex];
    particle.Active = true;
    particle.Position = particleProps.Position;
    particle.Rotation = particleProps.Rotation;

    particle.Velocity = particleProps.Velocity;
    particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
    particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

    particle.ColorBegin = particleProps.ColorBegin;
    particle.ColorEnd = particleProps.ColorEnd;

    particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
    particle.SizeEnd = particleProps.SizeEnd;

    particle.LifeTime = particleProps.LifeTime;
    particle.LifeRemaining = particleProps.LifeTime;

    m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
}

void ParticleSystem::OnUpdate()
{
    for (auto& particle : m_ParticlePool)
    {
        if (!particle.Active)
            continue;

        if (particle.LifeRemaining <= 0.0f)
        {
            particle.Active = false;
            continue;
        }

        particle.LifeRemaining -= XH::Time::deltatime;
        particle.Position += particle.Velocity * XH::Time::deltatime;
        particle.Rotation += 0.01f * XH::Time::deltatime;
    }
}

void ParticleSystem::OnRender()
{
    for (auto& particle : m_ParticlePool)
    {
        if (!particle.Active)
            continue;
        float life = particle.LifeRemaining / particle.LifeTime;
        glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);
        color.a *= life;

        float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);
        XH::Renderer2D::DrawQuad(particle.Position, { size, size }, color, particle.Rotation);
    }
}
