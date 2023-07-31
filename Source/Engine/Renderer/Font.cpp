#include "Font.h"

#include <iostream>
#include <ostream>
#include <SDL2-2.28.0/include/SDL_ttf.h>

namespace kiko
{
	Font::Font(const std::string& filename, int fontSize)
	{
		Font::Load("ArcadeClassic.ttf", 14);
	}

	Font::~Font()
	{
		if (m_ttfFont != NULL)
		{
			TTF_CloseFont(m_ttfFont); // Close the TTF font if it's not null
			m_ttfFont = nullptr;
		}
	}

	void Font::Load(const std::string& filename, int fontSize)
	{
		// Load the font using TTF_OpenFont and store the font in m_ttfFont
		m_ttfFont = TTF_OpenFont(filename.c_str(), fontSize);

		if (m_ttfFont == nullptr)
		{
			// Handle error if font loading fails
			//prints as a error message. should update to a try or whatever c++ version is later.
			std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
		}
	}
}