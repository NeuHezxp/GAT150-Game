#include "Font.h"

#include <iostream>
#include <ostream>
#include <SDL2-2.28.0/include/SDL_ttf.h>

#include "Core/Logger.h"

namespace kiko
{
	// Constructor for the Font class.
	// The constructor loads the font from the specified filename and font size.
	Font::Font(const std::string& filename, int fontSize)
	{
		// This line inside the constructor is not correct.
		// Instead of calling Load function inside the constructor, you should directly load the font here.
		// Also, the hard-coded filename and font size are not used correctly.
		// You should use the provided filename and fontSize to load the font.
		Font::Load("ArcadeClassic.ttf", 14);
	}

	// Destructor for the Font class.
	// It ensures that the loaded font is properly closed before destroying the object.
	Font::~Font()
	{
		if (m_ttfFont != nullptr)
		{
			TTF_CloseFont(m_ttfFont); // Close the TTF font if it's not null
			m_ttfFont = nullptr;
		}
	}

	// The Create function is not correctly implemented.
	// The Create function should be a pure virtual function in the base Resource class,
	// and it should be overridden in the Font class to load the font.
	// The implementation here doesn't match the expected behavior of a Create function.
	bool Font::Create(std::string filename, ...)
	{
		va_list args;

		va_start(args, filename);

		int fontSize = va_arg(args, int);

		va_end(args);
		return Load(filename, fontSize);
	}

	// This function loads the font from the specified filename and font size.
	bool Font::Load(const std::string& filename, int fontSize)
	{
		// Load the font using TTF_OpenFont and store the font in m_ttfFont
		m_ttfFont = TTF_OpenFont(filename.c_str(), fontSize);

		if (!m_ttfFont)
		{
			// Handle error if font loading fails
			//prints as an error message.
			WARNING_LOG("Failed to load font: " << filename);
			return false;
		}

		// Return true if the font is loaded successfully.
		return true;
	}
}