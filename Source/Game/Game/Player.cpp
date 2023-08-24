#include "Player.h"

#include "Weapon.h"
#include "Framework/Actor.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"

#include "SpaceGame.h"
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
		///new
		auto collisionComponent = GetComponent<kiko::CollisionComponent>();
		if (collisionComponent)
		{
			auto renderComponent = GetComponent<kiko::RenderComponent>();
			if (renderComponent)
			{
				float scale = transform.scale;
				collisionComponent->m_radius = renderComponent->GetRadius() * scale;
			}
		}
		return true; 
	}
	void Player::Update(float dt)
	{
		Actor::Update(dt);
		//movement
		float rotate = 0;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
		//transform.rotation += rotate * turnRate * g_time.GetDeltaTime();
		m_physicsComponent->ApplyTorque(rotate * turnRate);

		float thrust = 0;
		if (g_inputSystem.GetKeyDown(SDL_SCANCODE_W))
		{
			thrust = 1;
		}
		kiko::vec2 forward = vec2{ 0, -1 }.Rotate(transform.rotation);


		
		m_physicsComponent->ApplyForce(forward * speed * thrust);

		transform.position.x = Wrap(transform.position.x, (float)g_renderer.getWidth());
		transform.position.y = Wrap(transform.position.y, (float)g_renderer.getHeight());

		float reverseThrust = 0; // backward movement
		if (g_inputSystem.GetKeyDown(SDL_SCANCODE_S)) {
			reverseThrust = 1;
		}
		kiko::vec2 backward = kiko::vec2{ 0, 1 }.Rotate(transform.rotation);

		kiko::vec2 netForce = forward * speed * (thrust - reverseThrust);

		// Update position with the calculated net force
		transform.position += netForce * kiko::g_time.GetDeltaTime();

		//fire weapon
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
		{//create weapon
			auto weapon = INSTANTIATE(Weapon, "Rocket");
			weapon->transform = { transform.position, transform.rotation, 2.0f };
			weapon->Initialize();
			m_scene->Add(std::move(weapon));
		}
	}

	void Player::OnCollisionEnter(Actor* other)
	{
		if (other->tag == "Enemy") // could be a enemies bullet
		{
			destroyed = true;
			kiko::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);
		}
	}

	void Player::Read(const json_t& value)
	{
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_DATA(value, turnRate);
	}

}