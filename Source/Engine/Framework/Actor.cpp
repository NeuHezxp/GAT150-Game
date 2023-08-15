#include "Actor.h"

#include "Components/RenderComponent.h"

namespace kiko
{
	// This function updates the actor's state based on the time elapsed since the last update (dt).
	// If the actor has a lifespan set (m_lifespan != -1.0f), it decrements the lifespan and checks if it should be destroyed.
	// The actor's position is updated based on its velocity and damping (deceleration) over time (dt).
	void Actor::Update(float dt)
	{
		if (m_lifespan != -1.0f)
		{
			m_lifespan -= dt;
			m_destroyed = (m_lifespan <= 0);
		}
		for (auto& component : m_components) ///fix a issue with unique ptr and make it & actor
		{
			component->Update(dt);
		}
	}

	// This function draws the actor on the screen using the provided renderer.
	// It calls the Draw function of the actor's model, passing the renderer and the actor's transform as arguments.
	void Actor::Draw(kiko::Renderer& renderer)
	{
		for (auto& component : m_components) ///fix a issue with unique ptr and make it & actor
		{
			RenderComponent* renderComponent = dynamic_cast<RenderComponent*>(component.get());
			if (renderComponent)
			{
				renderComponent->Draw(renderer);
			}
		}
	}

	void Actor::AddComponent(std::unique_ptr<Component> component)
	{
		component->m_owner = this;
		m_components.push_back(std::move(component));
	}

	bool Actor::Initialize()
	{
		for (auto& component : m_components)
		{
			component->Initialize();
		}

		return true;
	}

	void Actor::OnDestroy()
	{
		for (auto& component : m_components)
		{
			component->OnDestroy();
		}
	}
}