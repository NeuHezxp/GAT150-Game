#pragma once
#include <string>
#include <SDL2-2.28.0/include/SDL_render.h>
#include <SDL2-2.28.0/include/SDL_video.h>
#include <SDL2-2.28.0/include/SDL.h>

namespace kiko
{
	// Function prototype for creating an SDL window
	void CreateWindow(const std::string& title, int width, int height);

	// Renderer class responsible for managing rendering
	class Renderer
	{
	public:
		// Default constructor and destructor
		Renderer() = default;
		~Renderer() = default;
		// Initialize the renderer (must be called before using the renderer)
		bool Initialize();
		// Shut down the renderer and free resources
		void Shutdown();
		// Alias for Shutdown() (redundant function)
		void shutdown();

		// Create an SDL window for rendering
		void CreateWindow(const std::string& title, int width, int height);
		// Begin a frame for rendering (clears the screen)
		void BeginFrame();
		// End a frame for rendering (presents the rendered image on the screen)
		void EndFrame();

		// Set the color of the renderer using rgb values
		void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
		//draw lines between points
		void DrawLine(int x1, int y1, int x2, int y2);
		void DrawLine(float x1, float y1, float x2, float y2);
		//draw points
		void DrawPoint(int x, int y);
		void DrawPoint(float x, float y);

		//draw texture with x,y and optinal angle
		void DrawTexture(class Texture* texture, float x, float y, float angle = 0.0f);

		//getters
		int getWidth() const { return m_width; }
		int getHeight() const { return m_height; }
		SDL_Renderer* getRenderer() const { return m_renderer; }

	private:
		//width height of renderer
		int m_width = 0;
		int m_height = 0;

		SDL_Renderer* m_renderer = nullptr;
		SDL_Window* m_window = nullptr;

		friend class Text;
	};
	extern Renderer g_renderer;
}
