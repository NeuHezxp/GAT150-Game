#pragma once

#include <SDL2-2.28.0/include/SDL_render.h>

#include "Core/vector2.h" // Include the vector2 header file which presumably contains the definition of the vec2 class.
#include "Framework/Resource/Resource.h"

struct SDL_Texture; // Forward declaration of the SDL_Texture struct, indicating that it will be used but not defined in this header.

namespace kiko
{
	// Declaration of the Texture class.
	class Texture : public Resource
	{
	public:
		Texture() = default; // Default constructor for the Texture class.
		~Texture(); // Destructor for the Texture class.

		virtual bool Create(std::string filename, ...) override; // Function to create a texture from an image file.
		bool Load(const std::string& filename, class Renderer& renderer); // Function to load a texture from a file.
		vec2 GetSize(); // Function to get the size (width and height) of the texture.

		SDL_Texture* GetTexture() const { return m_texture; } // Public getter function to access the m_texture member variable.

		friend class Renderer; // Declaring the Renderer class as a friend, allowing it to access private members of Texture.


	protected:
		SDL_Texture* m_texture = nullptr; // Pointer to the SDL_Texture, initialized to nullptr.
		// Inherited via Resource
	};	
}
