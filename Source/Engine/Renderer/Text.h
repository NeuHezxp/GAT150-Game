#pragma once
#include "Font.h"
#include "Core/Color.h"
#include <string>
#include <memory>
struct SDL_Texture;
namespace kiko
{
	class Renderer;

	class Text
	{
	public:
		Text() = default; //constructor shared ptr gotta look that one up
		Text(std::shared_ptr<Font>font) : m_font{ font } {}
		~Text(); //destructor
		void Create(Renderer& renderer, const std::string& text, const Color& color); //method with Renderer reference and the string and color rgb values
		void Draw(Renderer& renderer, int x, int y);  //draws with x, y cords
		void Draw(Renderer& renderer, const class Transform& transform);
	private:
		std::shared_ptr<Font> m_font;  //shared ptr font thing
		SDL_Texture* m_texture;
	};
}