#include "Player.h"


#include "Framework/Actor.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"


#include "Framework/Framework.h"
#include "Framework/Game.h"
#include "Framework/Components/circlecollisioncomponent.h"
#include "Framework/Components/CollisionComponent.h"
#include "Framework/Components/PhysicsComponent.h"
#include "Framework/Components/SpriteComponent.h"
#include "Framework/Resource/ResourceManager.h"

#include "Renderer/Renderer.h"

namespace kiko
{
	CLASS_DEFINITION(Player)

		bool Player::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<kiko::PhysicsComponent>();
		
		return true;
	}
	void Player::Update(float dt)
	{
		Actor::Update(dt);

		//movement
		float dir = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) dir = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) dir = 1;
		//transform.rotation += dir * turnRate * g_time.GetDeltaTime();
		//m_physicsComponent->ApplyTorque(dir * turnRate);

		kiko::vec2 forward = vec2{ 1, 0 };



		m_physicsComponent->ApplyForce(forward * speed * dir);

		transform.position.x = Wrap(transform.position.x, (float)g_renderer.getWidth());
		transform.position.y = Wrap(transform.position.y, (float)g_renderer.getHeight());
		

		//jump
		bool onGround = (groundCount > 0);
		if (onGround && kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
		{//create weapon
			kiko::vec2 up = kiko::vec2{ 0, -1 };
			m_physicsComponent->SetVelocity(up * jump);
		}
	}

	void Player::OnCollisionEnter(Actor* other)
	{
		if (other->tag == "Enemy") // could be a enemies bullet
		{
			destroyed = true;
			kiko::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);
		}
		if (other->tag == "Ground") groundCount++;
		
	}
	void Player::OnCollisionExit(Actor* other)
	{
		if (other->tag == "Ground") groundCount--;

	}

	void Player::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_DATA(value, jump);
	}

}