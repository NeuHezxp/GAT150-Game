#pragma once
#include "Core/Color.h"
#include "Core/vector2.h"

namespace kiko
{
	class Renderer;

	struct ParticleData
	{
		kiko::vec2 position;
		kiko::vec2 prevPosition;
		kiko::vec2 velocity;
		float damping = 0;
		kiko::Color color;
		float lifetimer = 0;
		float lifetime = 0;
	};

	class Particle
	{
	public:
		Particle() = default;
		void Initialize(const ParticleData& data)
		{
			m_data = data;
			m_isActive = true;
		}
		void Update(float dt);
		void Draw(kiko::Renderer& renderer);
		friend class ParticleSystem;
	private:
		ParticleData m_data;
		bool m_isActive = false;
	};
}
