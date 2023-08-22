#pragma once
#include "Framework/Actor.h"

namespace kiko {
	
	class Enemy : public kiko::Actor
	{
	public:
		CLASS_DECLARATION(Enemy)
		Enemy(float speed, float turnRate, const kiko::Transform& transform) :
			Actor{ transform },
			speed{ speed },
			turnRate{ turnRate }//without it calls the default constructor
		{
			fireRate = 2.0f;
			fireTimer = fireRate;
		}
		Enemy() = default;

		void Update(float dt) override; //dt

		void OnCollision(Actor* other) override;

		bool Initialize() override;

	protected:
		float speed = 0;
		float turnRate = 0;

		float fireRate = 0;
		float fireTimer = 0;
	};
}