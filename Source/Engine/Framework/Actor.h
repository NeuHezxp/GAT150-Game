#pragma once
#include "Object.h"
#include "Components/Component.h"
#include "Renderer/Model.h"
#include "Core/Core.h"
#include "Framework/Components/PhysicsComponent.h"

namespace kiko
{
	class Scene;

	class Actor : public Object // public to access object
	{
	public:
		Actor() = default;
		Actor(const Transform& transform) :
			m_transform{ transform } {}
		virtual bool Initialize() override;
		virtual void OnDestroy() override;

		virtual  void Update(float dt); //dt for delta time /// pure virtual aka: abstract
		virtual  void Draw(Renderer& renderer);

		void AddComponent(std::unique_ptr<Component> component);
		template<typename T>
		T* GetComponent();

		float GetRadius() { return 30.0f; }
		virtual	void OnCollision(Actor* other) {}

		class Scene* m_scene = nullptr;
		class Game* m_game = nullptr;

		friend class Scene;

		Transform m_transform;
		std::string m_tag;
		float m_lifespan = -1.0f;

	protected:
		std::vector<std::unique_ptr<Component>> m_components;
		bool m_destroyed = false; //flag
	};
	template<typename T>
	inline T* Actor::GetComponent()
	{
		for (auto& component : m_components)
		{
			T* result = dynamic_cast<T*>(component.get());
			if (result) return result;
		}

		return nullptr;
	}
}
