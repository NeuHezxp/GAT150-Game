

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

            float angle = kiko::vec2::SignedAngle(forward, direction.Normalized());
            m_physicsComponent->ApplyTorque(angle);
        
        
        
            Enemy* enemy = m_scene->GetActor<Enemy>();
            if (enemy && enemy != this)
            {
                kiko::Vector2 direction = enemy->transform.position - transform.position;
                transform.rotation = direction.Angle() + kiko::HalfPi;

                float angle = kiko::vec2::SignedAngle(forward, direction.Normalized());
                m_physicsComponent->ApplyTorque(angle);

                // Debug statement
                std::cout << "Enemy" << " targeting Enemy " << enemy << std::endl;
            }
            else
            {
                // Debug statement
                std::cout << "Enemy " << " has no target" << std::endl;
            }
        }


        // Movement
        float rotate = 0;

        // Check if the enemy is moving forward before allowing rotation
        if (currentSpeed > 0 || currentSpeed < -1)
        {
            // Gradual increase in rotation speed when turning
            if (currentSpeed < 0 || currentSpeed < -1)
            {
                rotate = -0.01 * (1.0 + currentSpeed / speed);
            }
            if (currentSpeed < 0 || currentSpeed < -1)
            {
                rotate = 0.01 * (1.0 + currentSpeed / speed);
            }
        }

        // Apply rotation to the enemy
        m_physicsComponent->ApplyTorque(rotate * turnRate);

        // Gradual acceleration
        currentSpeed = std::min(currentSpeed + acceleration * dt, speed);

        // Apply max speed limit if exceeded
        currentSpeed = std::min(currentSpeed, maxSpeed);

        // Apply forward force based on current speed
        m_physicsComponent->ApplyForce(forward * currentSpeed);

		
	}

	void Enemy::OnCollisionEnter(Actor* other)
	{

		if (other->tag == "Player" || "Enemy" && !this) // could be an enemy or enemy bullet
		{
			float damage = currentSpeed * speedToDamageFactor;
			// Reduce player's health
			health -= damage;
			if (health <= 0)
			{
				destroyed = true;
				// Emit particles for collision effect
				kiko::EmitterData data;
				data.burst = true;
				data.burstCount = 300;
				data.spawnRate = 200;
				data.angle = 0;
				data.angleRange = kiko::Pi;
				data.lifetimeMin = 0.1f;
				data.lifetimeMax = 0.3f;
				data.speedMin = 50;
				data.speedMax = 250;
				data.damping = 0.8f;
				data.color = kiko::Color{ 1, 0, 1, 1 };
				kiko::Transform emitterTransform{ transform.position, 0, 1 };
				auto emitter = std::make_unique<kiko::Emitter>(emitterTransform, data);
				emitter->lifespan = 1.0f;
				m_scene->Add(std::move(emitter));

				// Check if player's health is depleted
				if (health <= 0)
				{
					// Handle player's death
					destroyed = true;
				}

			}
		}
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