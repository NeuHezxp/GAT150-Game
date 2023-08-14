#pragma once
#include "RenderComponent.h"
#include "Renderer/texture.h"

namespace kiko
{
	class SpriteComponent : public RenderComponent
	{
	public:
		void Update(float dt) override;
		void Draw(class Renderer& renderer) override;
		virtual float GetRadius() { return m_texture->GetSize().length() * .5f; }
	public:
		res_t<Texture> m_texture; //member variable for texture
	};

}
