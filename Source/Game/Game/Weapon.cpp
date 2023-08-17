#include "Weapon.h"
#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Framework/framework.h"
#include "Framework/Components/CollisionComponent.h"

namespace kiko

{
	void Weapon::Update(float dt)
	{

		kiko::vec2 forward = kiko::vec2(0, -1).Rotate(transform.rotation);
		transform.position += forward * speed * kiko::g_time.GetDeltaTime();

		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_renderer.getWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_renderer.getHeight());
	}

	void Weapon::OnCollision(Actor* other)
	{
		if (other->tag != tag)
		{
			destroyed;
		}
	}


	bool Weapon::Initialize()
	{
		auto collisionComponent = GetComponent<CollisionComponent>();
		if (collisionComponent)
		{
			auto renderComponent = GetComponent<RenderComponent>();
			if ( renderComponent)
			{
				float scale = transform.scale;
				collisionComponent->m_radius = renderComponent->GetRadius() * scale;
			}
		}

		return true;
	}
	void Weapon::Read(const json_t& value)
	{
	}
}