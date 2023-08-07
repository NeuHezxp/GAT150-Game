#pragma once
#include "RenderComponent.h"
#include "Renderer/texture.h"

namespace kiko
{
	class SpriteComponent : public RenderComponent
	{
	public:
		void Update(float dt) ;
		void Draw(class Renderer& renderer) override;
	public:
		res_t<Texture> m_texture;
	};

}
