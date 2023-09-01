#include "Player.h"

#include "Audio/AudioSystem.h"
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
	//Player for CarDerby
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
		// Check if the car is moving forward before allowing rotation
		if (currentSpeed > 0 || currentSpeed <-1)
		{
			if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A))
			{
				rotate = -0.01 * (1.0 + currentSpeed / speed); // Gradual increase in rotation speed
			}
			if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D))
			{
				rotate = 0.01 * (1.0 + currentSpeed / speed); // Gradual increase in rotation speed
			}
		}
		/*if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -.01;
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = .01;*/
		
		m_physicsComponent->ApplyTorque(rotate * turnRate);

		float thrust = 0;
		if (g_inputSystem.GetKeyDown(SDL_SCANCODE_W))
		{
			thrust = 1;
			kiko::g_audioSystem.PlayOneShot("CarEngine", false);
		}
		if (g_inputSystem.GetKeyDown(SDL_SCANCODE_S))
		{
			thrust = -1;
		}

		// Apply acceleration and deceleration based on thrust
		if (thrust > 0)
		{
			currentSpeed = std::min(currentSpeed + acceleration * dt, speed);
		}
		else if (thrust < 0)
		{
			currentSpeed = std::max(currentSpeed - acceleration * dt, -speed);
		}
		else
		{
			// Decelerate when no input is pressed
			float decelerationFactor = (currentSpeed > 0) ? -deceleration : deceleration;
			currentSpeed = std::max(currentSpeed + decelerationFactor * dt, 0.0f);
		}
		// Apply max speed limit if exceeded
		currentSpeed = std::min(currentSpeed, maxSpeed);

		// Calculate forward force based on current speed
		kiko::vec2 forward = vec2{ 0, -1 }.Rotate(transform.rotation);
		m_physicsComponent->ApplyForce(forward * currentSpeed);

		/*transform.position.x = Wrap(transform.position.x, (float)g_renderer.getWidth());
		transform.position.y = Wrap(transform.position.y, (float)g_renderer.getHeight());*/

		float reverseThrust = 0; // backward movement
		if (g_inputSystem.GetKeyDown(SDL_SCANCODE_S)) {
			reverseThrust = 1;
		}
		kiko::vec2 backward = kiko::vec2{ 0, 1 }.Rotate(transform.rotation);

		// Apply brake force when the space key is pressed
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			// Adjust this value to control the braking force
			float brakeForce = 10.0f; // Adjust as needed
			currentSpeed = std::max(currentSpeed - brakeForce * dt, 0.0f);
		}
	}

	void Player::OnCollisionEnter(Actor* other)
	{

		if (other->tag == "Enemy") // could be an enemy or enemy bullet
		{
			float damage = currentSpeed * speedToDamageFactor;
			// Reduce player's health
			health -= damage;
			if (health <= 0)
			{
				destroyed = true;
				kiko::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);
				// Emit particles for collision effect
				kiko::EmitterData data;
				data.burst = true;
				data.burstCount = 100;
				data.spawnRate = 200;
				data.angle = 0;
				data.angleRange = kiko::Pi;
				data.lifetimeMin = 0.1f;
				data.lifetimeMax = 0.5f;
				data.speedMin = 50;
				data.speedMax = 250;
				data.damping = 0.5f;
				data.color = kiko::Color{ 1, 0, 0, 1 };
				kiko::Transform emitterTransform{ transform.position, 0, 1 };
				auto emitter = std::make_unique<kiko::Emitter>(emitterTransform, data);
				emitter->lifespan = 1.0f;
				m_scene->Add(std::move(emitter));

				// Check if player's health is depleted
				if (health <= 0)
				{
					// Handle player's death
					destroyed = true;
					kiko::EventManager::Instance().DispatchEvent("OnPlayerDead", 0);
				}
			}
				
				
		}
	}
	

	void Player::Read(const json_t& value)
	{
		Actor::Read(value);
		READ_DATA(value, speed);
		READ_DATA(value, turnRate);
	}

}