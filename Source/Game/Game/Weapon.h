#pragma once
#include "Framework/Actor.h"

namespace kiko
{
	class Weapon : public Actor
	{
	public:
		CLASS_DECLARATION(Weapon)

		bool Initialize() override;
		void Update(float dt) override; //dt for delta time
		void OnCollision(Actor* other);
		//void Read(const json_t& value);
	protected:
		float speed = 0;
	};
}
