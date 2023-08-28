#include "SpriteAnimRenderComponent.h"

#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"
#include "Renderer/Renderer.h"

namespace kiko
{
	// Definition of the SpriteAnimRenderComponent class
	CLASS_DEFINITION(SpriteAnimRenderComponent)

		// Initialize the sprite animation component
		bool SpriteAnimRenderComponent::Initialize()
	{
		// Call the base class Initialize function
		SpriteComponent::Initialize();
		// Set the default animation sequence
		SetSequence(defaultSequenceName);
		// Update the source image
		UpdateSource();

		return true;
	}

	// Update function for sprite animation
	void SpriteAnimRenderComponent::Update(float dt)
	{
		// Decrease the frame timer by the elapsed time
		frameTimer -= dt;
		if (frameTimer <= 0)
		{
			// Reset the frame timer based on sequence's fps
			frameTimer = 1.0f / m_Sequence->fps;
			// Advance to the next frame
			frame++;
			// Check if frame exceeds the sequence's end frame
			if (frame > m_Sequence->endFrame)
			{
				// Check if the sequence loops or stays at the end frame
				frame = (m_Sequence->loop) ? m_Sequence->startFrame : m_Sequence->endFrame;
			}
		}
		// Update the source image
		UpdateSource();
	}

	// Set a new animation sequence
	void SpriteAnimRenderComponent::SetSequence(const std::string& name)
	{
		// If the requested sequence is already active, do nothing
		if (m_Sequence && m_Sequence->name == name) return;

		// Check if the requested sequence exists
		if (m_sequences.find(name) != m_sequences.end())
		{
			// Set the new sequence
			m_Sequence = &m_sequences[name];
			// Set the texture if available in the sequence
			if (m_Sequence->texture) m_texture = m_Sequence->texture;
			// Reset frame information
			frame = m_Sequence->startFrame;
			frameTimer = 1.0f / m_Sequence->fps;
		}
	}

	// Update the source image based on current frame
	void SpriteAnimRenderComponent::UpdateSource()
	{
		// Calculate the size of an individual frame
		vec2 cellSize = m_texture->GetSize() / vec2(m_Sequence->numColumns, m_Sequence->numRows);
		// Calculate the current column index of the frame
		int column = (frame - 1) % m_Sequence->numColumns;
		// Calculate the current row index of the frame
		int row = (frame - 1) % m_Sequence->numRows;

		source.x = (int)(column * cellSize.x);
		source.y = (int)(row * cellSize.y);
		source.w = (int)(cellSize.x);
		source.h = (int)(cellSize.y);
	}

	// Read animation data from a JSON object
	void SpriteAnimRenderComponent::Read(const json_t& value)
	{
		// Call the base class Read function
		SpriteComponent::Read(value);

		// Read animation sequences from JSON
		if (HAS_DATA(value, sequences) && GET_DATA(value, sequences).IsArray())
		{
			for (auto& sequenceValue : GET_DATA(value, sequences).GetArray())
			{
				AnimSequence sequence;

				// Extract sequence data from JSON
				READ_NAME_DATA(sequenceValue, "name", sequence.name);
				READ_NAME_DATA(sequenceValue, "fps", sequence.fps);
				READ_NAME_DATA(sequenceValue, "numColumns", sequence.numColumns);
				READ_NAME_DATA(sequenceValue, "numRows", sequence.numRows);
				READ_NAME_DATA(sequenceValue, "startFrame", sequence.startFrame);
				READ_NAME_DATA(sequenceValue, "endFrame", sequence.endFrame);

				// Read texture name from JSON
				std::string textureName;
				READ_DATA(sequenceValue, textureName);
				// Load texture resource based on texture name
				sequence.texture = GET_RESOURCE(Texture, textureName, g_renderer);

				// Store the sequence in the sequences map
				m_sequences[sequence.name] = sequence;
			}
		}

		// Set default sequence
		if (!READ_DATA(value, defaultSequenceName))
		{
			// If default sequence not specified, use the first sequence in the sequences map
			defaultSequenceName = m_sequences.begin()->first;
		}
	}
}
