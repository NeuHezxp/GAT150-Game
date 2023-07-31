#pragma once
#include "Renderer/Model.h"
#include "Core/Core.h"


namespace kiko
{
	class Scene;

	class Actor
	{
	public:
		Actor() = default;
		Actor(const Transform& transform) :
			m_transform{ transform } {}
		Actor(const Transform& transform, std::shared_ptr<Model> model) :
			m_transform{ transform },
			m_model{ model }

		{}
		virtual  void Update(float dt); //dt for delta time /// pure virtual aka: abstract
		virtual  void Draw(Renderer& renderer);

		float GetRadius() { return (m_model) ? m_model->GetRadius() * m_transform.scale : 0; }
		virtual	void OnCollision(Actor* other) {}

		//complex physics
		void AddForce(vec2 force) { m_velocity += force; } // adds force to objects
		void SetDamping(float damping) { m_damping = damping; } //slows down objects over time

		class Scene* m_scene = nullptr;
		class Game* m_game = nullptr;

		friend class Scene;

		Transform m_transform;
		std::string m_tag;
		float m_lifespan = -1.0f;

	protected:
		bool m_destroyed = false; //flag

		std::shared_ptr<Model> m_model;

		vec2 m_velocity;
		float m_damping = 0;

	};
}
