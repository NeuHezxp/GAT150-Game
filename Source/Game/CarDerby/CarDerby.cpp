#include "CarDerby.h"
#include "CarDerby/Player.h"
#include "CarDerby/Enemy..h"
#include "Audio/AudioSystem.h"
#include "Framework/Factory.h"
#include "Input/InputSystem.h"
//#include "Framework/Components/RenderComponent.h"

#include "Renderer/Renderer.h"
#include "Framework/Framework.h"

// This function is responsible for initializing the game state.
// It creates font and text objects for displaying the score, title, game over text, timer, and winner text.
// It also loads background music and other audio files.
// Additionally, it initializes the game scene.
bool CarDerby::Initialize()
{
	// create font / text objects
	m_font = GET_RESOURCE(kiko::Font, "text/arcadeclassic.ttf", 24);
	m_scoreText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "text/arcadeclassic.ttf", 24));
	m_scoreText->Create(kiko::g_renderer, "Score 0000", kiko::Color{ 1, 1, 1, 1 });

	m_titleText = std::make_unique<kiko::Text>(m_font);
	m_titleText->Create(kiko::g_renderer, "CarDerby", kiko::Color{ 1, 1, 1, 1 });

	m_gameovertext = std::make_unique<kiko::Text>(m_font);
	m_gameovertext->Create(kiko::g_renderer, "GameOVER", kiko::Color{ 1, 1, 1, 1 });

	m_timerText = std::make_unique<kiko::Text>(m_font);
	m_timerText->Create(kiko::g_renderer, "Timer", kiko::Color{ 1, 1, 1, 1 });

	m_winnerText = std::make_unique<kiko::Text>(m_font);
	m_winnerText->Create(kiko::g_renderer, "You Win", kiko::Color{ 1, 1, 1, 1 });

	//load background music
	kiko::g_audioSystem.AddAudio("background", "Audio/background.ogg");
	//loading menu audio
	kiko::g_audioSystem.AddAudio("menu", "restart.wav");

	//loading audio
	kiko::g_audioSystem.AddAudio("CarEngine", "audio/Carengine.wav");
	kiko::g_audioSystem.AddAudio("hit", "audio/laser_shoot.wav");
	kiko::g_audioSystem.AddAudio("dash", "audio/dashecho.wav");

	//create scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("scenes/CarDerbyScene.json");
	m_scene->Initialize();
	EVENT_SUBSCRIBE("OnAddPoints", CarDerby::OnAddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", CarDerby::OnPlayerDead);

	return true;
}

bool CarDerby::Shutdown()
{
	return false;
}

void CarDerby::Update(float dt)
{
	switch (m_state)
	{
	case CarDerby::eState::Title:
			m_scene->GetActorByName<kiko::Actor>("Title")->active = true;
			m_scene->GetActorByName<kiko::Actor>("Wintext")->active = false;
		//play menu audio
			kiko::g_audioSystem.PlayOneShot("background", true);

		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			m_scene->GetActorByName<kiko::Actor>("Background")->active = false;
			m_state = eState::StartGame;
			m_scene->GetActorByName<kiko::Actor>("GameBackground")->active = false;

			kiko::g_audioSystem.StopSounds();
		}
		//reset score/timer
		m_score = 0;
		m_gameTimer = 0.0f;
		break;
	case CarDerby::eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_state = eState::StartLevel;
		break;

	case CarDerby::eState::StartLevel:
		m_scene->RemoveAll();

		{
			//Create Player

			auto player = INSTANTIATE(kiko::Player, "Player");
			player->transform = kiko::Transform({ 750,550 }, 0, .5);
			player->Initialize();
			m_scene->Add(std::move(player));

		


			m_scene->GetActorByName("Title")->active = true;

		}
		m_state = eState::Game;
		break;

	case CarDerby::eState::Game:
		m_scene->GetActorByName("Title")->active = false;
		m_scene->GetActorByName<kiko::Actor>("GameBackground")->active = true;
		m_gameTimer += dt; // a timer that counts down
		m_spawnTimer += dt;
		//if the player lasts long enough
		if (m_gameTimer >= 30.0f) 
		{
			m_state = eState::Winner;
			m_scene->GetActorByName<kiko::Actor>("GameBackground")->active = false;
			
		}
		
		if (m_spawnTimer >= m_spawnTime)
		{
			m_spawnTimer = 0;

			// Spawn enemies in each corner
			for (int i = 0; i < 3; ++i)
			{
				kiko::Vector2 spawnPosition;

				// Determine spawn position for each corner
				if (i == 0) // Top-left corner
					spawnPosition = { 25, 30 };
				else if (i == 1) // Top-right corner
					spawnPosition = {  600, 30 };
				else if (i == 2) // Bottom-left corner
					spawnPosition = { 0, kiko::g_renderer.getHeight() -50 };

				auto enemy = INSTANTIATE(kiko::Enemy, "Enemy");
				enemy->transform = kiko::Transform(spawnPosition, 1, 0.5);
				enemy->Initialize();
				m_scene->Add(std::move(enemy));
			}
			
		}
		break;
	case eState::PlayerDeadStart:
		m_stateTimer = 5;
		m_state = eState::PlayerDead;
		m_stateTimer -= dt;

		break;
	case CarDerby::eState::PlayerDead:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0)
		{
			m_state = eState::Title;
		}
		break;
	case CarDerby::eState::GameOver:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0)
		{
			m_scene->RemoveAll();
			m_state = eState::Title;
		}
		break;
	case CarDerby::eState::Winner:
		m_scene->RemoveAll();
		m_scene->GetActorByName<kiko::Actor>("Crate")->active = false;

		
		if (!m_waiting) // Check if the waiting process has started
		{
			m_scene->GetActorByName<kiko::Actor>("Wintext")->active = true;
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
	m_winnerText->Create(kiko::g_renderer, "Winner", { 1, 1, 1, 1 });
	m_scene->Update(dt);
}

void CarDerby::Draw(kiko::Renderer& renderer)
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
	m_scene->Draw(renderer);

	m_scoreText->Draw(renderer, 40, 40);
	kiko::g_particleSystem.Draw(renderer);
}
void CarDerby::OnAddPoints(const kiko::Event& event)
{
	m_score += std::get<int>(event.data);
}
void CarDerby::OnPlayerDead(const kiko::Event& event)
{
	m_lives--;
	m_state = eState::PlayerDeadStart;
}
