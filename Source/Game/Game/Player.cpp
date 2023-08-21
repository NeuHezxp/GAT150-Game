#include "Player.h"

#include "Weapon.h"
#include "Framework/Actor.h"
#include "Framework/Scene.h"
#include "Input/InputSystem.h"

#include "SpaceGame.h"
#include "Framework/Framework.h"
#include "Framework/Game.h"
//#include "Framework/Components/circlecollisioncomponent.h"
#include "Framework/Components/CollisionComponent.h"
#include "Framework/Components/PhysicsComponent.h"
#include "Framework/Components/SpriteComponent.h"
#include "Framework/Resource/ResourceManager.h"
#include "Renderer/Renderer.h"

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
	return true; // does work
}
void Player::Update(float dt)
{
	Actor::Update(dt);
	std::cout << transform.position.x << transform.position.y << std::endl;
	//movement
	float rotate = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	transform.rotation += rotate * m_turnRate * kiko::g_time.GetDeltaTime();

	float thrust = 0;
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_W))
	{
		thrust = 1;
		std::cout << "moving" <<std::endl;
	}
	kiko::vec2 forward = kiko::vec2{ 0, -1 }.Rotate(transform.rotation);
	

	///VVVVVS
	m_physicsComponent->ApplyForce(forward * m_speed * thrust);

	transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.getWidth());
	transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.getHeight());

	float reverseThrust = 0; // backward movement
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_S)) {
		reverseThrust = 1;
	}
	kiko::vec2 backward = kiko::vec2{ 0, 1 }.Rotate(transform.rotation);

	kiko::vec2 netForce = forward * m_speed * (thrust - reverseThrust);

	//Addforce(netForce);

	// Update position with the calculated net force
	transform.position += netForce * kiko::g_time.GetDeltaTime();

	auto player = INSTANTIATE(Player, "player")
//	player->m_game = this;
	player->Initialize();
	m_scene->Add(std::move(player));

	//fire weapon
	if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_inputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE))
	{//create weapon
	}
	//	kiko::Transform transform{transform.position, transform.rotation, 1}; //include degrees to radians 10.0f + 1
	//	std::unique_ptr<kiko::WeaponComponent> laser = std::make_unique<kiko::WeaponComponent>(400.0f, transform);//m_model is the weapons model !!change this. //m_model change to transform
	//	laser->tag = "PlayerLaser";
	//	std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
	//	component->m_texture = GET_RESOURCE(kiko::Texture,"rocket.png", kiko::g_renderer);

	//	auto collisionComponent = std::make_unique<kiko::CircleCollisionComponent>();
	//	collisionComponent->m_radius = 30.0f;
	//	laser->AddComponent(std::move(collisionComponent));
	//	laser->Initialize();
	//	m_scene->Add(std::move(laser));
	//}

	//if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_T)) kiko::g_time.SetTimeScale(.1f);
	//else kiko::g_time.SetTimeScale(1);
}

void Player::OnCollision(Actor* other)
{
	if (other->tag == "Enemy") // could be a enemies bullet
	{
		m_game->setLives(m_game->Getlives() - 1);
		destroyed = true;
		dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDeadStart);
	}
}