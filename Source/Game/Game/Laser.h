#pragma once
#include "Framework/Actor.h"
#include "Framework/Framework.h"

namespace kiko
{
	class Laser : public Actor
	{
	public:
		Laser(float speed, const kiko::Transform& transform) :
			Actor{ transform },
			m_speed{ speed }
		{
			m_lifespan = 2.0f;
		}

		void Update(float dt) override; //dt for delta time
		void OnCollision(Actor* other) override;
		bool Initialize() override;
	protected:
		bool m_shoulddestroy = false;
		float m_speed = 0;
	};
}
