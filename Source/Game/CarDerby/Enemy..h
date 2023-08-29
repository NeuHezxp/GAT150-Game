
#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"

namespace kiko {

	class Enemy : public kiko::Actor
	{
	public:
		CLASS_DECLARATION(Enemy)
			Enemy() = default;

		void Update(float dt) override; //dt

		void OnCollisionEnter(Actor* other) override;

		bool Initialize() override;

	protected:
		float speed = 0;
		float turnRate = 0;


		kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}