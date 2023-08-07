#include "Renderer.h"
#include <SDL2-2.28.0/include/SDL.h>

#include "texture.h"
#include "Core/vector2.h"
#include "SDL2-2.28.0/include/SDL_ttf.h"
#include "SDL2-2.28.0/include/SDL_image.h"

namespace kiko
{
	// Global instance of Renderer (singleton pattern)
	Renderer g_renderer;
	// Global SDL_Renderer pointer
	SDL_Renderer* renderer{ nullptr };
	// Global SDL_Window pointer
	SDL_Window* window{ nullptr };

	// Function to create an SDL window
	void CreateWindow(const std::string& title, int width, int height)
	{
		// Initialize SDL_VIDEO subsystem
		SDL_Init(SDL_INIT_VIDEO);

		// Create an SDL window with the specified title, position, width, height, and flags
		window = SDL_CreateWindow(title.c_str(), 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		// Create an SDL renderer for the window with hardware acceleration and vertical sync
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	}

	// Renderer class methods

	// Initialize the Renderer (called before using the Renderer)
	bool Renderer::Initialize()
	{
		// Initialize SDL_VIDEO and other subsystems for image and text rendering
		SDL_Init(SDL_INIT_VIDEO);
		IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
		TTF_Init();
		return true;
	}

	// Shutdown the Renderer and release resources
	void Renderer::Shutdown()
	{
		// Destroy the SDL renderer and window
		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);

		// Quit TTF (TrueType Font) and IMG (image loading) subsystems
		TTF_Quit();
		IMG_Quit();
	}

	// Create an SDL window for rendering
	void Renderer::CreateWindow(const std::string& title, int width, int height)
	{
		// Set the width and height of the renderer window
		m_width = width;
		m_height = height;

		// Create an SDL window with the specified title, position, width, height, and flags
		m_window = SDL_CreateWindow(title.c_str(), 100, 100, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		// Create an SDL renderer for the window with hardware acceleration and vertical sync
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);
	}

	// Begin a frame for rendering (clears the screen)
	void Renderer::BeginFrame()
	{
		SDL_RenderClear(m_renderer);
	}

	// End a frame for rendering (presents the rendered image on the screen)
	void Renderer::EndFrame()
	{
		SDL_RenderPresent(m_renderer);
	}

	// Set the color for rendering (r, g, b, and a values range from 0 to 255)
	void Renderer::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
	{
		SDL_SetRenderDrawColor(m_renderer, r, g, b, a);
	}

	// Draw a line from (x1, y1) to (x2, y2) (integer version)
	void Renderer::DrawLine(int x1, int y1, int x2, int y2)
	{
		SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
	}

	// Draw a line from (x1, y1) to (x2, y2) (floating-point version)
	void Renderer::DrawLine(float x1, float y1, float x2, float y2)
	{
		SDL_RenderDrawLineF(m_renderer, x1, y1, x2, y2);
	}

	// Draw a point at the specified coordinates (floating-point version)
	void Renderer::DrawPoint(float x, float y)
	{
		SDL_RenderDrawPointF(m_renderer, x, y);
	}

	// Draw a texture at the specified position with an optional angle
	void Renderer::DrawTexture(Texture* texture, float x, float y, float angle)
	{
		// Get the size of the texture
		vec2 size = texture->GetSize();

		// Create a destination rectangle for rendering the texture
		SDL_Rect dest;
		dest.x = static_cast<int>(x) -size.x * .5f; //shifting origin
		dest.y = static_cast<int>(y) -size.y * .5f; //shifting origin
		dest.w = static_cast<int>(size.x);
		dest.h = static_cast<int>(size.y);

		// Render the texture with an optional rotation angle
		SDL_RenderCopyEx(m_renderer, texture->GetTexture(), nullptr, &dest, angle, nullptr, SDL_FLIP_NONE);
	}

	// Draw a point at the specified coordinates
	void Renderer::DrawPoint(int x, int y)
	{
		SDL_RenderDrawPoint(m_renderer, x, y);
	}
}