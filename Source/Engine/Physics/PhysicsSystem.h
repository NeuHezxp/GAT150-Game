#pragma once
#include "PhysicsSystem.h" // Include Singleton
#include "../ThirdParty/box2d/include/box2d/b2_world.h" // Include b2World header
#include <memory>
#include "Core/vector2.h"
#include "Framework/Singleton.h"
#include <box2d/include/box2d/box2d.h>

//macros
#define VEC2_TO_B2VEC2(vec) (*(b2Vec2*)(&vec)) // Convert vec2 to b2Vec2
#define B2VEC2_TO_VEC2(vec) (*(kiko::Vector2*)(&vec))// Convert b2Vec2 to vec2
namespace kiko
{
	class PhysicsSystem : public Singleton<PhysicsSystem> // class that handles physics
    {
	public:
		struct RigidBodyData// rigid body data
		{
			float gravityScale = 1;
			float damping = 0;
			float angularDamping = 0;
			bool constrainAngle = false;
			bool isDynamic = true;
		};

		struct CollisionData
		{
			vec2 size;
			float density = 1;
			float friction = 1;
			float restitution = 0.3f;
			bool isTrigger = false;
		};
    public:
        bool Initialize();
        void Update(float dt);
		b2Body* CreateBody(const vec2& position, float angle, const RigidBodyData& data);
		void DestroyBody(b2Body* body);

		void SetCollisionBox(b2Body* body, const CollisionData& data, class Actor* actor = nullptr);
		void SetCollisionBoxStatic(b2Body* body, const CollisionData& data, class Actor* actor = nullptr);

		vec2 WorldToScreen(const vec2& world) { return world * m_pixelsPerUnit; }
		vec2 ScreenToWorld(const vec2& screen) { return screen * (1.0f / m_pixelsPerUnit); }

        friend class Singleton; // Declare Singleton as a friend class
    private:
        PhysicsSystem() = default; // Declare default constructor
    private:
		float m_pixelsPerUnit = 48.0f;
		std::unique_ptr<b2World> m_world; // Declare b2World
    };
} // namespace kiko
