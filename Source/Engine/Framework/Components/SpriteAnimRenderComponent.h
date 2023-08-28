#pragma once
#include "SpriteComponent.h"

namespace kiko
{
	// Declaration of the SpriteAnimRenderComponent class
	class SpriteAnimRenderComponent : public SpriteComponent
	{
	public:
		// Structure to hold animation sequence information
		struct AnimSequence
		{
			std::string name;

			float fps = 0;         // Frames per second of the animation
			int numColumns = 0;    // Number of columns in the sprite sheet
			int numRows = 0;       // Number of rows in the sprite sheet

			int startFrame = 0;    // Starting frame of the sequence
			int endFrame = 0;      // Ending frame of the sequence

			bool loop = true;      // Whether the animation should loop

			std::shared_ptr<Texture> texture;  // Pointer to the texture used for this sequence
		};

	public:
		// Macro to declare the class as a SpriteAnimRenderComponent
		CLASS_DECLARATION(SpriteAnimRenderComponent);

		// Initialize the component
		bool Initialize() override;

		// Update the animation
		void Update(float dt);

		// Set the active animation sequence
		void SetSequence(const std::string& name);

		// Update the source image for rendering
		void UpdateSource();

	public:
		// Data members for managing the current frame and timer
		int frame = 0;           // Current frame of the animation
		float frameTimer = 0;    // Timer for controlling frame updates

	private:
		// Name of the default sequence
		std::string defaultSequenceName;

		// Map to store animation sequences
		std::map<std::string, AnimSequence> m_sequences;

		// Pointer to the currently active animation sequence
		AnimSequence* m_Sequence = nullptr;
	};
}
