#include "WeaponComponent.h"
#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Framework/framework.h"
#include "Framework/Components/CollisionComponent.h"

namespace kiko

{
	void WeaponComponent::Update(float dt)
	{

		kiko::vec2 forward = kiko::vec2(0, -1).Rotate(m_owner->transform.rotation);
		m_owner->transform.position += forward * speed * kiko::g_time.GetDeltaTime();

		m_owner->transform.position.x = kiko::Wrap(m_owner->transform.position.x, (float)kiko::g_renderer.getWidth());
		m_owner->transform.position.y = kiko::Wrap(m_owner->transform.position.y, (float)kiko::g_renderer.getHeight());
	}

	void WeaponComponent::OnCollision(Actor* other)
	{
		if (other->tag != m_owner->tag)
		{
			m_owner->destroyed;
		}
	}


	bool WeaponComponent::Initialize()
	{
		auto collisionComponent = m_owner->GetComponent<CollisionComponent>();
		if (collisionComponent)
		{
			auto renderComponent = m_owner->GetComponent<RenderComponent>();
			if ( renderComponent)
			{
				float scale = m_owner->transform.scale;
				collisionComponent->m_radius = renderComponent->GetRadius() * scale;
			}
		}

		return true;
	}
	void WeaponComponent::Read(const json_t& value)
	{
	}
}