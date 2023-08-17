#pragma once
#include "Framework/Actor.h"

namespace kiko
{
	class Weapon : public Actor
	{
	public:
		/*Weapon(float speed, const kiko::Transform& transform) :
			Actor{ transform },
			speed{ speed }
		{
			lifespan = 2.0f;
		}*/

		bool Initialize() override;
		void Update(float dt) override; //dt for delta time
		void OnCollision(Actor* other);
		void Read(const json_t& value);
	protected:
		float speed = 0;
	};
}
