#include "Weapon.h"
#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Framework/framework.h"
#include "Framework/Components/CollisionComponent.h"

namespace kiko

{
	CLASS_DEFINITION(Weapon)
		void Weapon::Update(float dt)
	{

		Actor::Update(dt);
		kiko::vec2 forward = kiko::vec2(0, -1).Rotate(transform.rotation);
		m_physicsComponent->SetVelocity(forward * speed);

		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.getWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.getHeight());
	}

	void Weapon::OnCollisionEnter(Actor* other)
	{
		if (other->tag != tag)
		{
			destroyed;
		}
	}

	bool Weapon::Initialize()
	{
		Actor::Initialize();
		m_physicsComponent = GetComponent<PhysicsComponent>();

		auto collisionComponent = GetComponent<CollisionComponent>();
		if (collisionComponent)
		{
		}

		return true;
	}
	void Weapon::Read(const json_t& value)
	{
		Actor::Read(value);
		READ_DATA(value, speed);
	}
}