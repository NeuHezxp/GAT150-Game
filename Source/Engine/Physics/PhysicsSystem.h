#pragma once
#include "PhysicsSystem.h" // Include Singleton
#include "../ThirdParty/box2d/include/box2d/b2_world.h" // Include b2World header
#include <memory>
#include "Framework/Singleton.h"

namespace kiko
{
    class PhysicsSystem : public Singleton<PhysicsSystem>
    {
    public:
        bool Initialize();
        void Update(float dt);
        friend class Singleton; // Declare Singleton as a friend class
    private:
        PhysicsSystem() = default; // Declare default constructor
    private:
        std::unique_ptr<b2World> m_world; // Use correct type for unique_ptr
    };
} // namespace kiko
