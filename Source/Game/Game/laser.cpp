#include "Laser.h"
#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Framework/framework.h"
#include "Framework/Components/CollisionComponent.h"

namespace kiko

{
	void Laser::Update(float dt)
	{
		Actor::Update(dt);

		kiko::vec2 forward = kiko::vec2(0, -1).Rotate(m_transform.rotation);
		m_transform.position += forward * m_speed * kiko::g_time.GetDeltaTime();

		m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_renderer.getWidth());
		m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_renderer.getHeight());
	}

	void Laser::OnCollision(Actor* other)
	{
		if (other->m_tag != m_tag)
		{
			m_destroyed = true;
		}
	}

	bool Laser::Initialize()
	{
		auto collisionComponent = GetComponent<kiko::CollisionComponent>();
		if (collisionComponent)
		{
			auto renderComponent = GetComponent<kiko::RenderComponent>();
			if (renderComponent)
			{
				float scale = m_transform.scale;
				collisionComponent->m_radius = renderComponent->GetRadius() * scale;
			}
		}

		return true;
	}
}
