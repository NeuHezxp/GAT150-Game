#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"

namespace kiko
{
	class Player : public kiko::Actor
	{
	public:
		CLASS_DECLARATION(Player)

		void OnCollision(Actor* other) override;
		bool Initialize() override;
		void Update(float dt) override; //dt for delta time
	private:
		kiko::PhysicsComponent* m_physicsComponent = nullptr;

	protected:
		float speed = 0;
		float turnRate = 0;
	};
}