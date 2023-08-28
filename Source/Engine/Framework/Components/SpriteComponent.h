#pragma once
#include "RenderComponent.h"
#include "Framework/Factory.h"

#include "Renderer/Texture.h"

namespace kiko
{
	// Declaration of the SpriteComponent class
	class SpriteComponent : public RenderComponent
	{
	public:
		// Macro to declare the class as a SpriteComponent
		CLASS_DECLARATION(SpriteComponent);

		// Initialize the sprite component
		bool Initialize() override;

		// Update function for sprite component
		void Update(float dt);

		// Draw function for sprite rendering
		void Draw(class Renderer& renderer);
	public:

		// Data member for the rectangle defining the source of the sprite
		Rect source;

		// Name of the texture used by the sprite
		std::string textureName;

		// Resource handle for the texture
		res_t<Texture> m_texture;  // member variable for texture
	};
}
