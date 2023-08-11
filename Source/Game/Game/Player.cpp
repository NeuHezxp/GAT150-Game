#include "Player.h"

#include "Laser.h"
#include "Framework/Actor.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"

#include "Laser.h"
#include "SpaceGame.h"
#include "Framework/Emitter.h"
#include "Framework/Game.h"
#include "Framework/Components/PhysicsComponent.h"
#include "Framework/Components/SpriteComponent.h"
#include "Framework/Resource/ResourceManager.h"
#include "Renderer/Renderer.h"

void Player::Update(float dt)
{
 	Actor::Update(dt);

	//movement
	float rotate = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	m_transform.rotation += rotate * m_turnRate * kiko::g_time.GetDeltaTime();

	float thrust = 0; // forward movement
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(m_transform.rotation);
	//AddForce(forward * m_speed * thrust);
	auto physicsComponent = GetComponent<kiko::PhysicsComponent>();
	physicsComponent->ApplyForce(forward * m_speed * thrust);


	//m_transform.position += forward * m_speed * thrust * kiko::g_time.GetDeltaTime();

	///for out of bounds
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.getWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.getHeight());

	float reverseThrust = 0; // backward movement
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_S)) {
		reverseThrust = 1;
	}
	kiko::vec2 backward = kiko::vec2{ 0, 1 }.Rotate(m_transform.rotation);

	kiko::vec2 netForce = forward * m_speed * (thrust - reverseThrust);

	//Addforce(netForce);

	// Update position with the calculated net force
	m_transform.position += netForce * kiko::g_time.GetDeltaTime();

	// *Need to finish*Clamp the speed so it doesn't exceed the maximum speed in either direction

	/////Adding special movement
	//if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_LSHIFT))
	//{
	//	m_isDashing = true;
	//	kiko::vec2 dashDirection = forward;
	//	//AddForce(dashDirection *= m_dashSpeed); // Add force in the dash direction
	//}

	//if (m_isDashing)
	//{
	//	m_dashDuration -= kiko::g_time.GetDeltaTime();
	//	if (m_dashDuration <= 0.0f)
	//	{
	//		m_isDashing = false;
	//		m_dashDuration = 0.1f; // Reset the dash duration for the next dash
	//		std::cout << "dashing" << std::endl;
	//	}
	//	//create explosion on death
	//	kiko::EmitterData data;
	//	data.burst = true;
	//	data.burstCount = 100;
	//	data.spawnRate = 0;
	//	data.angle = 0;
	//	data.angleRange = kiko::Pi;
	//	data.lifetimeMin = 0.5f;
	//	data.lifetimeMax = 1.5f;
	//	data.speedMin = 50;
	//	data.speedMax = 250;
	//	data.damping = 0.5f;
	//	data.color = kiko::Color{ 1, 0, 0, 1 };
	//	kiko::Transform transform{ m_transform.position, 0, 1 };
	//	auto emitter = std::make_unique<kiko::Emitter>(transform, data);
	//	emitter->m_lifespan = 1.0f;
	//	m_scene->Add(std::move(emitter));
	//}

	//Actor::Update(dt);

	//fire weapon
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
	{
		//create weapon
		kiko::Transform transform{m_transform.position, m_transform.rotation, 1}; //include degrees to radians 10.0f + 1
		std::unique_ptr<kiko::Laser> laser = std::make_unique<kiko::Laser>(400.0f, transform);//m_model is the weapons model !!change this. //m_model change to transform
		laser->m_tag = "PlayerLaser";
		std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
		component->m_texture = kiko::g_resources.Get<kiko::Texture>("rocket.png", kiko::g_renderer);
		laser->AddComponent(std::move(component));
		m_scene->Add(std::move(laser));
	}

	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) kiko::g_time.SetTimeScale(.1f);
	else kiko::g_time.SetTimeScale(1);
}

void Player::OnCollision(Actor* other)
{
	if (other->m_tag == "Enemy") // could be a enemies bullet
	{
		m_game->setLives(m_game->Getlives() - 1);
		m_destroyed = true;
		dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDeadStart);
	}
}