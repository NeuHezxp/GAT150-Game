#pragma once

namespace kiko
{
	class Component
	{
public:
		virtual ~Component() = default;
		virtual void Update(float dt) = 0;

	friend class Actor;

	protected:
		class Actor* m_owner = nullptr;
	};

}

