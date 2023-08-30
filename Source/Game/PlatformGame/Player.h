#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"
#include "Framework/framework.h"

namespace kiko
{
	class Player : public kiko::Actor
	{
	public:
		CLASS_DECLARATION(Player)

		bool Initialize() override;
		void Update(float dt) override; //dt for delta time

		void OnCollisionEnter(Actor* other) override;
		void OnCollisionExit(Actor* other) override;
	private:
		kiko::PhysicsComponent* m_physicsComponent = nullptr;

	protected:
		float speed = 0;
		float jump = 0;
		int groundCount = 0;
		class SpriteAnimRenderComponent* m_spriteAnimComponent = nullptr;
	};
}