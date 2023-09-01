#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"

namespace kiko
{
	class Player : public kiko::Actor
	{
	public:
		CLASS_DECLARATION(Player)

			void OnCollisionEnter(Actor* other) override;
		bool Initialize() override;
		void Update(float dt) override; //dt for delta time
	private:
		kiko::PhysicsComponent* m_physicsComponent = nullptr;

	protected:
		int health = 10;
		float maxSpeed = 20;
		float acceleration = 0.1f;
		float deceleration = 0.1f;
		float currentSpeed = 0.0f;
		float speed = 0;
		const float speedToDamageFactor = 0.01f;
		float turnRate = 0;
	};
}