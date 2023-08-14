#pragma once
#include "Framework/Object.h"

namespace kiko
{
                      
    class Component : public Object
    {
    public:
        virtual void Update(float dt) = 0;

        friend class Actor;
        Actor* GetOwner() { return m_owner; }
        ///made public
    public:
        class Actor* m_owner = nullptr;
    };
}