#include "SpaceGame.h"
#include "Framework/Scene.h"
#include "Player.h"
#include "Enemy.h"
#include "Audio/AudioSystem.h"
#include "Framework/Emitter.h"
#include "Input/InputSystem.h"
#include "Renderer/ModelManager.h"
#include "Renderer/Renderer.h"
#include "Renderer/Text.h"
#include "Renderer/ModelManager.h"

bool SpaceGame::Initialize()
{
	// create font / text objects
	m_font = std::make_shared<kiko::Font>("arcadeclassic.ttf", 24);
	m_scoreText = std::make_unique<kiko::Text>(m_font);
	m_scoreText->Create(kiko::g_renderer, "Score 0000", kiko::Color{ 1, 1, 1, 1 });

	m_titleText = std::make_unique<kiko::Text>(m_font);
	m_titleText->Create(kiko::g_renderer, "asteroid", kiko::Color{ 1, 1, 1, 1 });

	m_gameovertext = std::make_unique<kiko::Text>(m_font);
	m_gameovertext->Create(kiko::g_renderer, "GameOVER", kiko::Color{ 1, 1, 1, 1 });

	m_timerText = std::make_unique<kiko::Text>(m_font);
	m_timerText->Create(kiko::g_renderer, "Timer", kiko::Color{ 1, 1, 1, 1 });

	m_winnerText = std::make_unique<kiko::Text>(m_font);
	m_winnerText->Create(kiko::g_renderer, "You Win", kiko::Color{ 1, 1, 1, 1 });

	//load background music
	kiko::g_audioSystem.AddAudio("background", "background.wav");
	//loading menu audio
	kiko::g_audioSystem.AddAudio("menu", "restart.wav");

	//loading audio
	kiko::g_audioSystem.AddAudio("explosion", "explosion.wav");
	kiko::g_audioSystem.AddAudio("hit", "laser_shoot.wav");
	kiko::g_audioSystem.AddAudio("dash", "dashecho.wav");

	//
	m_scene = std::make_unique<kiko::Scene>();
	kiko::Scene scene;

	return true;
}

bool SpaceGame::Shutdown()
{
	return false;
}

void SpaceGame::Update(float dt)
{
	switch (m_state)
	{
	case SpaceGame::eState::Title:
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			m_state = eState::StartGame;

		}
		//reset score/timer
		m_score = 0;
		m_gameTimer = 0.0f;
		break;
	case SpaceGame::eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_state = eState::StartLevel;
		break;

	case SpaceGame::eState::StartLevel:
	{
		m_scene->RemoveAll();
		std::unique_ptr<Player> player = std::make_unique<Player>(10.0f, kiko::Pi, kiko::Transform{ {400, 300}, 0, 4 }, kiko::g_manager.Get("ship.txt"));
		player->m_tag = "Player";
		player->m_game = this;
		player->SetDamping(0.9f); // slows player down when floating
		player->m_game = this;
		m_scene->Add(std::move(player));
	}
	m_state = eState::Game;
	break;

	case SpaceGame::eState::Game:
		m_gameTimer += dt; // a timer that counts down
		m_spawnTimer += dt;
		//if the player lasts long enough
		if (m_gameTimer >= 30.0f)
		{
			m_state = eState::Winner;
		}

		// Restart the game on 'R' key press
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_R))
		{
			kiko::g_audioSystem.PlayOneShot("menu");
			m_state = eState::Title;
			m_scene->RemoveAll();
		}

		if (m_spawnTimer >= m_spawnTime)
		{
			// spawn timer back to 0
			m_spawnTimer = 0;
			// creates enemy after the timer hits 0
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(kiko::randomf(75.0f, 150.0f), kiko::Pi, kiko::Transform{ {kiko::random(kiko::g_renderer.getWidth()), kiko::random(kiko::g_renderer.getHeight())}, kiko::randomf(kiko::TwoPi), 4 }, kiko::g_manager.Get("enemyship.txt"));
			enemy->m_tag = "Enemy";
			enemy->m_game = this;
			m_scene->Add(std::move(enemy));

			/// pressed mouse spawns particle
			if (kiko::g_inputSystem.GetMouseButtonDown(0))
			{
				kiko::g_audioSystem.PlayOneShot("laser");
				/// emitter stuff
				kiko::EmitterData data;
				data.burst = true;
				data.burstCount = 100;
				data.spawnRate = 200;
				data.angle = 0;
				data.angleRange = kiko::Pi;
				data.lifetimeMin = 0.5f;
				data.lifetimeMax = 1.5f;
				data.speedMin = 50;
				data.speedMax = 250;
				data.damping = 0.5f;
				data.color = kiko::Color{ 1, 0, 0, 1 };
				kiko::Transform transform{ {kiko::g_inputSystem.GetMousePosition() }, 0, 1 };
				auto emitter = std::make_unique<kiko::Emitter>(transform, data);
				emitter->m_lifespan = 1.0f;
				m_scene->Add(std::move(emitter));
			} // emitter stuff
		}
		break;
	case eState::PlayerDeadStart:
		m_stateTimer = 3;
		m_state = eState::PlayerDead;
		m_stateTimer -= dt;

		break;
	case SpaceGame::eState::PlayerDead:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0)
		{
			m_state = eState::StartLevel;
		}
		break;
	case SpaceGame::eState::GameOver:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0)
		{
			m_scene->RemoveAll();
			m_state = eState::Title;
		}
		break;
	case SpaceGame::eState::Winner:
		m_scene->RemoveAll();
		if (!m_waiting) // Check if the waiting process has started
		{
			m_waiting = true; // Set the waiting flag to true
			m_stateTimer = 1.0f; // Set the waiting time to 1 second (or any desired value)
		}
		else
		{
			m_stateTimer -= dt;
			if (m_stateTimer <= 0)
			{
				m_waiting = false; // Reset the waiting flag
				m_scene->RemoveAll();
				m_state = eState::Title; // Move to the Title state
			}
		}
		break;
	default:
		break;
	}
	m_scoreText->Create(kiko::g_renderer, std::to_string(m_score), { 1, 1, 1, 1 });
	m_timerText->Create(kiko::g_renderer, std::to_string((int)m_gameTimer), { 1, 1, 1, 1 });
	m_winnerText->Create(kiko::g_renderer, ("Winner"), { 1, 1, 1, 1 });
	m_scene->Update(dt);
}

void SpaceGame::Draw(kiko::Renderer& renderer)
{
	if (m_state == eState::Title)
	{
		m_titleText->Draw(renderer, 400, 300);
	}
	if (m_state == eState::GameOver)
	{
		m_gameovertext->Draw(renderer, 600, 400);
	}
	if (m_state == eState::Winner)
	{
		m_winnerText->Draw(kiko::g_renderer, 400, 200);
	}
	m_timerText->Draw(renderer, 400, 40);

	m_scoreText->Draw(renderer, 40, 40);
	m_scene->Draw(renderer);
	kiko::g_particleSystem.Draw(renderer);
}