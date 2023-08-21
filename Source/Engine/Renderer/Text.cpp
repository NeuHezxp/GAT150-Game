#include "Text.h"
#include "Font.h"
#include <SDL2-2.28.0/include/SDL_ttf.h>
#include "Renderer/Renderer.h"

namespace kiko
{
	// Destructor for the Text class
	Text::~Text()
	{
		if (m_texture) SDL_DestroyTexture(m_texture);  // If a texture exists, destroy it to free resources
	}

	// Create a text texture
	void Text::Create(Renderer& renderer, const std::string& text, const Color& color)
	{
		if (m_texture) SDL_DestroyTexture(m_texture);  // Destroy existing texture if it exists

		// Convert the given color to SDL_Color format
		SDL_Color c{ Color::toInt(color.r), Color::toInt(color.g), Color::toInt(color.b), Color::toInt(color.a) };

		// Render the given text onto an SDL surface using the specified font and color
		SDL_Surface* surface = TTF_RenderText_Solid(m_font->m_ttfFont, text.c_str(), c);

		// Create an SDL texture from the rendered surface
		m_texture = SDL_CreateTextureFromSurface(renderer.m_renderer, surface);

		// Free the surface since the texture has been created
		SDL_FreeSurface(surface);
	}

	// Draw the text on the renderer at the specified position
	void Text::Draw(Renderer& renderer, int x, int y)
	{
		int width, height;

		// Get the dimensions of the texture
		SDL_QueryTexture(m_texture, nullptr, nullptr, &width, &height);

		// Define a rectangle to specify the position and size of the rendered text
		SDL_Rect rect{ x, y, width, height };

		// Render the text texture on the provided renderer using the defined rectangle
		SDL_RenderCopy(renderer.m_renderer, m_texture, nullptr, &rect);
	}

	void Text::Draw(Renderer& renderer, const Transform& transform)
	{
		int width, height;
		SDL_QueryTexture(m_texture, nullptr, nullptr, &width, &height);
		mat3 mx = transform.GetMatrix();
		vec2 position = mx.GetTranslation();
		vec2 size = vec2{ width, height } *mx.GetScale();
		SDL_Rect dest;
		dest.x = (int)(position.x - (size.x / 2));
		dest.y = (int)(position.y - (size.y / 2));
		dest.w = static_cast<int>(size.x);
		dest.h = static_cast<int>(size.y);
		// https://wiki.libsdl.org/SDL2/SDL_RenderCopyEx
		SDL_RenderCopyEx(renderer.m_renderer, m_texture, nullptr, &dest, RadiansToDegrees(mx.GetRotation()), nullptr, SDL_FLIP_NONE);
	}

}
