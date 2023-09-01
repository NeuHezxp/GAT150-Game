
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
		int health = 5;
		float speed = 0;
		float turnRate = 0;
		float acceleration = 0.1f; // Added this line for acceleration for the cars
		float currentSpeed = 0.0f; // Added this line to track current speed of cars
		float maxSpeed = 20.0f;
		const float speedToDamageFactor = 0.01f;

		kiko::Actor* m_target = nullptr; // This is the target the enemy will chase


		kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}