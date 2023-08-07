#include "Enemy.h"

#include "Laser.h"
#include "Player.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Enemy.h"
#include "Laser.h"
#include "Framework/Game.h"
#include "SpaceGame.h"
#include "Audio/AudioSystem.h"
#include "Framework/Emitter.h"

void Enemy::Update(float dt)
{
	Actor::Update(dt);

	kiko::vec2 forward = kiko::vec2{0, -1}.Rotate(m_transform.rotation);
	Player* player = m_scene->GetActor<Player>();
	if (player)
	{
		kiko::Vector2 direction = player->m_transform.position - m_transform.position;
		m_transform.rotation = direction.Angle() + kiko::HalfPi;


		float angle = kiko::vec2::SignedAngle(forward, direction.Normalized());
		if (std::fabs(angle) < kiko::DegreesToRadians(30.0f))
		{
			// fire
		}
	}

	m_transform.position += forward * m_speed * kiko::g_time.GetDeltaTime();
	m_transform.position.x = kiko::Wrap(m_transform.position.x, static_cast<float>(kiko::g_renderer.getWidth()));
	m_transform.position.y = kiko::Wrap(m_transform.position.y, static_cast<float>(kiko::g_renderer.getHeight()));

	//decrement the time and reset the timer // use fire timer  => 0 reset the fire timer
	if (m_fireTimer <= 0)
	{
		m_fireTimer = m_firerate;
		//create weapon
		kiko::Transform transform{m_transform.position, m_transform.rotation, .5f};
		std::unique_ptr<kiko::Laser> laser = std::make_unique<kiko::Laser>(400.0f, transform);
		m_scene->Add(std::move(laser));
	}
	else
	{
		m_fireTimer -= dt; // subtracting dt from m_fireTimer
	}
}

void Enemy::OnCollision(Actor* other)
{
	//dynamic_cast<Player*>(other); //can check for type

	if (other->m_tag == "PlayerLaser")
	{

			
			m_game->AddPoints(100);
			m_destroyed = true;
			kiko::g_audioSystem.PlayOneShot("explosion");

			
	}

}