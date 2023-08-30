#include "Enemy.h"

#include "Player.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Enemy.h"
#include "Weapon.h"
#include "Framework/Game.h"
#include "SpaceGame.h"
#include "Audio/AudioSystem.h"
#include "Framework/Emitter.h"

#include <Framework//Framework.h>

#include "Framework/Components/CollisionComponent.h"

namespace kiko {
	CLASS_DEFINITION(Enemy)
		void Enemy::Update(float dt)
	{
		Actor::Update(dt);
		

		m_physicsComponent = GetComponent<PhysicsComponent>();
		kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
		Player* player = m_scene->GetActor<Player>();
		if (player)
		{
			kiko::Vector2 direction = player->transform.position - transform.position;
		m_physicsComponent->ApplyForce(direction.Normalized() * speed); 
		}

		//transform.position += forward * speed * kiko::g_time.GetDeltaTime();
		transform.position.x = kiko::Wrap(transform.position.x, static_cast<float>(kiko::g_renderer.getWidth()));
		transform.position.y = kiko::Wrap(transform.position.y, static_cast<float>(kiko::g_renderer.getHeight()));
/*
		////decrement the time and reset the timer // use fire timer  => 0 reset the fire timer
		//if (fireTimer <= 0)
		//{
		//	fireTimer = fireRate;
		//	//create weapon
		//	kiko::Transform transform{transform.position, transform.rotation, .5f};
		//	std::unique_ptr<kiko::WeaponComponent> laser = std::make_unique<kiko::WeaponComponent>(400.0f, transform);
		//	m_scene->Add(std::move(laser));
		//}
		//else
		//{
		//	fireTimer -= dt; // subtracting dt from fireTimer
		//}
		*/
	}

	void Enemy::OnCollisionEnter(Actor* other)
	{
		//dynamic_cast<Player*>(other); //can check for type

		if (other->tag == "PlayerLaser")
		{
			EventManager::Instance().DispatchEvent("AddPoints", 100);
			m_game->AddPoints(100);
			destroyed = true;
			g_audioSystem.PlayOneShot("explosion");
		}
		kiko::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);
	}

	bool Enemy::Initialize()
	{
		Actor::Initialize();

		return true;
	}
	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);
		READ_DATA(value, speed);
		READ_DATA(value, turnRate);
		READ_DATA(value, fireRate);
	}

}