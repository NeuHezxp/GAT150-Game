#include <iostream>
#include <thread>
#include <vector>
#include "Core/Core.h"
#include <Renderer/Renderer.h>
#include <Renderer/Model.h>
#include "Input/InputSystem.h"
#include "Framework/Actor.h"
#include "Enemy.h"
#include "Player.h"
#include "Audio/AudioSystem.h"
#include "Framework/Scene.h"
#include "Renderer/Font.h"
#include "Renderer/Text.h"
#include "Renderer/ParticleSystem.h"
#include <thread>
#include <memory>
#include <Framework/Emitter.h>

#include "SpaceGame.h"
#include "Renderer/ModelManager.h"

using namespace std;

class Star
{
public:
	Star(const kiko::Vector2& pos, const kiko::Vector2& vel) :
		m_pos(pos),
		m_vel(vel)
	{}

	void Update()
	{
		m_pos += m_vel * kiko::g_time.GetDeltaTime();
	}

	void Draw(kiko::Renderer& renderer)
	{
		renderer.DrawPoint(m_pos.x, m_pos.y);
	}

public:
	kiko::vec2 m_pos;
	kiko::vec2 m_vel;
};
//leaving when adding in vector2.h before stream op for the number in {}
int main(int argc, char* argv[])
{
	// random stuff for memeory
	kiko::MemoryTracker::Initialize();

	auto m = kiko::Max(4.0f, 3.0f);
	auto m2 = kiko::Max(4, 3);

	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");

	//for video render
	kiko::g_renderer.Initialize();
	kiko::g_renderer.CreateWindow("CSC196", 800, 600);

	//for audio
	kiko::AudioSystem audioSystem;
	kiko::g_audioSystem.Initialize();
	//for global input
	kiko::g_inputSystem.Initialize();

	//create Game
	unique_ptr<SpaceGame> game = make_unique<SpaceGame>();
	game->Initialize();

	std::vector<Star> stars;

	float speed = 500;
	constexpr float turnRate = kiko::DegreesToRadians(360);

	///Main Loop
	bool quit = false;
	while (!quit)
	{
		///update engine
		kiko::g_time.Tick();
		kiko::g_inputSystem.Update();
		kiko::g_audioSystem.Update();

		kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());

		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE)) // using the escape key to break out of loop and exit program
		{
			quit = true;
		}
		kiko::g_audioSystem.PlayOneShot("background", true);//loops

		///plays a one shot of a sound declared above.
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			kiko::g_audioSystem.PlayOneShot("hit");
		}

		///update scene
		game->Update(kiko::g_time.GetDeltaTime());

		/// draw
		kiko::g_renderer.setColor(0, 0, 0, 0); // Sets color to black
		kiko::g_renderer.BeginFrame(); // Clears the screen, allows for less static

		//kiko::g_renderer.setColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
		kiko::g_renderer.setColor(255, 255, 255, 255);
		//adding text
		//text->Draw(kiko::g_renderer, 400, 300);
		// Update and draw stars
		for (auto& star : stars)
		{
			star.Update();

			if (star.m_pos.x >= kiko::g_renderer.getWidth())
				star.m_pos.x = 0;
			if (star.m_pos.y >= kiko::g_renderer.getHeight())
				star.m_pos.y = 0;

			kiko::g_renderer.DrawPoint(star.m_pos.x, star.m_pos.y);
		}
		//stars.clear();
		// Draw model at the current position and enemies around player
		game->Draw(kiko::g_renderer);
		kiko::g_particleSystem.Draw(kiko::g_renderer);

		kiko::g_renderer.EndFrame();
	}

	return 0;
}
