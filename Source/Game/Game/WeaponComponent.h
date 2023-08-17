#pragma once
#include "Framework/Actor.h"
#include "Framework/Framework.h"
#include "Framework/Components/Component.h"

namespace kiko
{
	class WeaponComponent : public Component
	{
	public:
		/*WeaponComponent(float speed, const kiko::Transform& transform) :
			Actor{ transform },
			speed{ speed }
		{
			lifespan = 2.0f;
		}*/

		void Update(float dt) override; //dt for delta time
		void OnCollision(Actor* other);
		bool Initialize() override;
		void Read(const json_t& value);
	protected:
		float speed = 0;
	};
}
