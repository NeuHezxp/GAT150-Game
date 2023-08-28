#include "Core/Core.h"
#include "Framework/Framework.h"
#include <Renderer/Renderer.h>
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Renderer/ParticleSystem.h"
#include <memory>
#include <cassert>
#include <functional>
#include <iostream>
#include <vector>
#include <thread>

#include "CarDerby.h"
#include "Core/Json.h"
#include "Framework/Factory.h"
#include "Physics/PhysicsSystem.h"
//#include <array>
//#include <map>

using namespace std;



int main(int argc, char* argv[])
{

	
	INFO_LOG("Initializing Main")
		// Initialize the game engine
		kiko::MemoryTracker::Initialize();
	kiko::seedRandom(static_cast<unsigned int>(time(nullptr)));
	kiko::setFilePath("assets/CarDerby");

	// Initialize the renderer
	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);

	// Initialize audio and input systems
	kiko::AudioSystem audioSystem;
	kiko::g_audioSystem.Initialize();
	kiko::g_inputSystem.Initialize();
	kiko::PhysicsSystem::Instance().Initialize();

	// Create the game
	auto game = make_unique<CarDerby>();
	game->Initialize();

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

		kiko::PhysicsSystem::Instance().Update(kiko::g_time.GetDeltaTime());

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
		//draws the particles
		kiko::g_particleSystem.Draw(kiko::g_renderer);
		// Draw the game
		game->Draw(kiko::g_renderer);

		

		// End the frame and present it
		kiko::g_renderer.EndFrame();
	}
	return 0;
}