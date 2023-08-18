#include "Actor.h"
#include "Components/RenderComponent.h"

namespace kiko
{
	CLASS_DEFINITION(Actor)

	bool Actor::Initialize()
	{
		for (auto& component : components)
		{
			component->Initialize();
		}

		return true;
	}

	void Actor::OnDestroy()
	{
		for (auto& component : components)
		{
			component->OnDestroy();
		}
	}

	// This function updates the actor's state based on the time elapsed since the last update (dt).
	// If the actor has a lifespan set (lifespan != -1.0f), it decrements the lifespan and checks if it should be destroyed.
	// The actor's position is updated based on its velocity and damping (deceleration) over time (dt).
	void Actor::Update(float dt)
	{
		if (lifespan != -1.0f)
		{
			lifespan -= dt;
			destroyed = (lifespan <= 0.0f);
		}
		for (auto& component : components) ///fix a issue with unique ptr and make it & actor
		{
			component->Update(dt);
		}
	}

	// This function draws the actor on the screen using the provided renderer.
	// It calls the Draw function of the actor's model, passing the renderer and the actor's transform as arguments.
	void Actor::Draw(kiko::Renderer& renderer)
	{
		for (auto& component : components) ///fix a issue with unique ptr and make it & actor
		{
			if (dynamic_cast<RenderComponent*>(component.get()))
			{
				dynamic_cast<RenderComponent*>(component.get())->Draw(renderer);
			}
		}
	}

	void Actor::AddComponent(std::unique_ptr<Component> component)
	{
		component->m_owner = this;
		components.push_back(std::move(component));
	}

	void Actor::Read(const json_t& value)
	{
		Object::Read(value);
		READ_DATA(value, tag);
		READ_DATA(value, lifespan);
		if (HAS_DATA(value, transform)) transform.Read(GET_DATA(value,transform));

		if (HAS_DATA(value, components) && GET_DATA(value, components).IsArray())
		{
			for (auto& componentValue : GET_DATA(value, components).GetArray())
			{
				std::string type;
				READ_DATA(componentValue,type);
				auto component = CREATE_CLASSBASE(Component, type);
				component->Read(componentValue);

				AddComponent(std::move(component));
			}
		}
	}


}