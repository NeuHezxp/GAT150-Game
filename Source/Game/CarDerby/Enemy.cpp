

#include "Player.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Framework/Game.h"
#include "CarDerby.h"
#include "Audio/AudioSystem.h"
#include "Framework/Emitter.h"

#include <Framework//Framework.h>

#include "Enemy..h"
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
			transform.rotation = direction.Angle() + kiko::HalfPi;
			//m_physicsComponent->ApplyTorque(turnAngle); fix this
			float angle = kiko::vec2::SignedAngle(forward, direction.Normalized());
		}

		m_physicsComponent->ApplyForce(forward * speed);
		//transform.position += forward * speed * kiko::g_time.GetDeltaTime();
		transform.position.x = kiko::Wrap(transform.position.x, static_cast<float>(kiko::g_renderer.getWidth()));
		transform.position.y = kiko::Wrap(transform.position.y, static_cast<float>(kiko::g_renderer.getHeight()));
		
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
	void Enemy::Read(const json_t& value)
	{
		Actor::Read(value);
		READ_DATA(value, speed);
		READ_DATA(value, turnRate);
	}

}