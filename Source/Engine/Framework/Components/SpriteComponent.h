#pragma once
#include "RenderComponent.h"
#include "Framework/Factory.h"

#include "Renderer/texture.h"

namespace kiko
{
	class SpriteComponent : public RenderComponent
	{
	public:
		CLASS_DECLARATION(SpriteComponent);

		void Update(float dt);
		void Draw(class Renderer& renderer);
		virtual float GetRadius() { return m_texture->GetSize().length() * .5f; }
	public:
		

		res_t<Texture> m_texture; //member variable for texture
	};
}
