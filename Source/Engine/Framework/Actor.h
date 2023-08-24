#pragma once
#include "Object.h"
#include "Components/Component.h"
#include "Renderer/Model.h"
#include "Core/Core.h"
#include "Framework/Components/PhysicsComponent.h"

namespace kiko
{
	class Scene;

	class Actor : public Object  //public to access object
	{
	public:
		CLASS_DECLARATION(Actor)

			Actor() = default;
		Actor(const Transform& transform) :
			transform{ transform } {}
		Actor(const Actor& other);

		virtual bool Initialize() override;
		virtual void OnDestroy() override;

		virtual  void Update(float dt);// dt for delta time / pure virtual aka: abstract
		virtual  void Draw(Renderer& renderer);

		void AddComponent(std::unique_ptr<Component> component);
		template<typename T>
		T* GetComponent();

		virtual	void OnCollisionEnter(Actor* other) {}
		virtual	void OnCollisionExit(Actor* other) {}

		class Scene* m_scene = nullptr;
		class Game* m_game = nullptr;

		friend class Scene;
	public:
		Transform transform;
		std::string tag;
		float lifespan = -1.0f;
		bool destroyed = false; //flag
		bool persistent = false;
		bool prototype = false;

	protected:
		std::vector<std::unique_ptr<Component>> components;
	};
	template<typename T>
	inline T* Actor::GetComponent()
	{
		for (auto& component : components)
		{
			auto result = dynamic_cast<T*>(component.get());
			if (result) return result;
		}

		return nullptr;
	}
}
