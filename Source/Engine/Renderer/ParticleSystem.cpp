#include "ParticleSystem.h"
#include "Core/Core.h"

void kiko::ParticleSystem::Update(float dt)
{
	for (auto& particle : m_particles)
	{
		if (particle.m_isActive) particle.Update(dt);
	}
}
void kiko::ParticleSystem::Draw(kiko::Renderer& renderer)
{
	for (auto& particle : m_particles)
	{
		if (particle.m_isActive) particle.Draw(renderer);
	}
}

kiko::Particle* kiko::ParticleSystem::GetFreeParticle()
{
	for (auto& particle : m_particles)
	{
		if (!particle.m_isActive) return &particle;
	}
	return nullptr;
}

kiko::ParticleSystem kiko::g_particleSystem(1000);