#include <vector>
#include "Framework/Framework.h"
#include "Core/Core.h"
#include <Renderer/Renderer.h>
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Renderer/ParticleSystem.h"
#include <memory>
#include "SpaceGame.h"
#include <cassert>


using namespace std;

// Star class represents a point in the game, moving with a velocity to 
class Star
{
public:
	Star(const kiko::Vector2& pos, const kiko::Vector2& vel) :
		m_pos(pos),
		m_vel(vel)
	{}

	void Update()
	{
		// Update the position based on velocity and time
		m_pos += m_vel * kiko::g_time.GetDeltaTime();
	}

	void Draw(kiko::Renderer& renderer)
	{
		// Draw the star as a point
		renderer.DrawPoint(m_pos.x, m_pos.y);
	}

public:
	kiko::vec2 m_pos;
	kiko::vec2 m_vel;
};


int main(int argc, char* argv[])
{

	INFO_LOG("Initializing Game")
		// Initialize the game engine
		kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");

	// Initialize the renderer
	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);

	// Initialize audio and input systems
	kiko::AudioSystem audioSystem;
	kiko::g_audioSystem.Initialize();
	kiko::g_inputSystem.Initialize();

	// Create the game
	auto game = make_unique<SpaceGame>();
	game->Initialize();


	// Create a vector to store stars
	std::vector<Star> stars;
	for (int i = 0; i < 1000; i++)
	{
		auto pos(kiko::Vector2(kiko::random(kiko::g_renderer.getWidth()), kiko::random(kiko::g_renderer.getHeight())));
		kiko::Vector2 vel(kiko::randomf(100, 200), 0.0f);

		stars.push_back(Star(pos, vel));
	}
	INFO_LOG("Updating engine Components in main")
	// Main Loop that runs the game
	bool quit = false;
	while (!quit)
	{
		
		// Update engine components
		kiko::g_time.Tick();
		kiko::g_inputSystem.Update();
		kiko::g_audioSystem.Update();
		kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());

		// Check for quit condition (using the escape key)
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}

		// Play background audio
		kiko::g_audioSystem.PlayOneShot("background", true);

		// Play a one-shot audio when space key is pressed
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			kiko::g_audioSystem.PlayOneShot("hit");
		}

		// Update the game scene
		game->Update(kiko::g_time.GetDeltaTime());

		// Set the renderer color to black and clear the screen
		kiko::g_renderer.setColor(0, 0, 0, 0);
		kiko::g_renderer.BeginFrame();

		// Draw the stars
		kiko::g_renderer.setColor(255, 255, 255, 255);
		
		
		// Draw the game elements and particles
		game->Draw(kiko::g_renderer);
		kiko::g_particleSystem.Draw(kiko::g_renderer);
		// Create a vector to store stars
		
		// Update and draw the stars
		for (auto& star : stars)
		{
			star.Update();

			if (star.m_pos.x >= kiko::g_renderer.getWidth()) star.m_pos.x = 0;
			if (star.m_pos.y >= kiko::g_renderer.getHeight()) star.m_pos.y = 0;

			kiko::g_renderer.setColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);  
			kiko::g_renderer.DrawPoint(star.m_pos.x, star.m_pos.y);
		}
		

		// End the frame and present it
		kiko::g_renderer.EndFrame();
	}
	stars.clear(); // clears the stars
	return 0;
}