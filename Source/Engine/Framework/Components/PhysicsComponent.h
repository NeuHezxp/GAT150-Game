#pragma once
#include "Component.h"
#include "Core/vector2.h"

namespace kiko
{
	class PhysicsComponent : public Component //base class to physics componets
	{
	public:
		void ApplyForce(const vec2& force);
	public:
		vec2 m_velocity;
		vec2 m_acceleration;
		float m_mass = 1.0f;
		float m_damping = 0;//slows objects down
	};
}