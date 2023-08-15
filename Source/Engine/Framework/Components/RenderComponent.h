#pragma once
#include "Component.h"

namespace kiko
{
	class RenderComponent : public kiko::Component
	{
	public:
		virtual void Draw(class Renderer& renderer) = 0;
		virtual float GetRadius() { return  0; }
	};
}
