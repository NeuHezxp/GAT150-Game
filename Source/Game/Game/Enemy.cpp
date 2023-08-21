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

void Enemy::Update(float dt)
{
	Actor::Update(dt);

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
	Player* player = m_scene->GetActor<Player>();
	if (player)
	{
		kiko::Vector2 direction = player->transform.position - transform.position;
		transform.rotation = direction.Angle() + kiko::HalfPi;

		float angle = kiko::vec2::SignedAngle(forward, direction.Normalized());
		if (std::fabs(angle) < kiko::DegreesToRadians(30.0f))
		{
			// fire
		}
	}

	transform.position += forward * m_speed * kiko::g_time.GetDeltaTime();
	transform.position.x = kiko::Wrap(transform.position.x, static_cast<float>(kiko::g_renderer.getWidth()));
	transform.position.y = kiko::Wrap(transform.position.y, static_cast<float>(kiko::g_renderer.getHeight()));

	////decrement the time and reset the timer // use fire timer  => 0 reset the fire timer
	//if (m_fireTimer <= 0)
	//{
	//	m_fireTimer = m_firerate;
	//	//create weapon
	//	kiko::Transform transform{transform.position, transform.rotation, .5f};
	//	std::unique_ptr<kiko::WeaponComponent> laser = std::make_unique<kiko::WeaponComponent>(400.0f, transform);
	//	m_scene->Add(std::move(laser));
	//}
	//else
	//{
	//	m_fireTimer -= dt; // subtracting dt from m_fireTimer
	//}
}

void Enemy::OnCollision(Actor* other)
{
	//dynamic_cast<Player*>(other); //can check for type

	if (other->tag == "PlayerLaser")
	{
		m_game->AddPoints(100);
		destroyed = true;
		kiko::g_audioSystem.PlayOneShot("explosion");

	}
}

bool Enemy::Initialize()
{
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