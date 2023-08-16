#include <cassert>

#include "texture.h"
#include "renderer.h"
#include "Core/Logger.h"
#include "SDL2-2.28.0/include/SDL_image.h"

namespace kiko
{
	// Destructor for Texture class
	Texture::~Texture()
	{
		// Destroy the SDL texture if it exists
		if (m_texture) SDL_DestroyTexture(m_texture);
	}

	bool Texture::Create(std::string filename, ...)
	{
		va_list args;

		va_start(args, filename);

		Renderer& renderer = va_arg(args, Renderer);

		va_end(args);

		return Load(filename, renderer);
	}

	// Create a texture from an image file
	bool Texture::Load(const std::string& filename, Renderer& renderer)
	{
		// Load the image file into an SDL surface
		SDL_Surface* surface = IMG_Load(filename.c_str());
		if (!surface)
		{
			// If loading the image fails, log a warning and return false
			WARNING_LOG("Failed to load texture: " << filename);
			return false;
		}

		// Create an SDL texture from the loaded surface
		m_texture = SDL_CreateTextureFromSurface(renderer.getRenderer(), surface);//fix this somewhere
		SDL_FreeSurface(surface);

		if (!m_texture)
		{
			// If creating the texture from the surface fails, log a warning and return false
			WARNING_LOG("Failed to create texture from surface for image: " << filename);
			return false;
		}

		return true;
	}

	// Get the size (width and height) of the texture
	vec2 Texture::GetSize() const
	{
		// ASSERT texture is not null (checks if the texture exists before proceeding)
		assert(m_texture != nullptr);

		// Get the width and height of the texture using SDL_QueryTexture
		SDL_Point point;
		SDL_QueryTexture(m_texture, nullptr, nullptr, &point.x, &point.y);

		// Return the size as a vec2 (casting to float to avoid data loss)
		return vec2{ static_cast<float>(point.x), static_cast<float>(point.y) };
	}
}