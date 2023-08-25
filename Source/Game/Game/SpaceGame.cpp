#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "Audio/AudioSystem.h"
#include "Framework/Factory.h"
#include "Input/InputSystem.h"
#include "Framework/Components/RenderComponent.h"

#include "Renderer/Renderer.h"
#include "Framework/Framework.h"

// This function is responsible for initializing the game state.
// It creates font and text objects for displaying the score, title, game over text, timer, and winner text.
// It also loads background music and other audio files.
// Additionally, it initializes the game scene.
bool SpaceGame::Initialize()
{
	// create font / text objects
	m_font = GET_RESOURCE(kiko::Font, "arcadeclassic.ttf", 24);
	m_scoreText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "arcadeclassic.ttf", 24));
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

	//create scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("scenes/spacescene.json");
	m_scene->Initialize();
	EVENT_SUBSCRIBE("OnAddPoints", SpaceGame::OnAddPoints); 
	EVENT_SUBSCRIBE("OnPlayerDead",SpaceGame::OnPlayerDead);

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
			m_scene->GetActorByName<kiko::Actor>("Background")->active = false;
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
		m_scene->RemoveAll();

	{
		//Create Player
		
		auto player = INSTANTIATE(kiko::Player, "Player");
		player->transform = kiko::Transform({ 400,300 }, 0, 1);
		player->Initialize();
		m_scene->Add(std::move(player));

		
		m_scene->GetActorByName("Title")->active = true;
		
	}
	m_state = eState::Game;
	break;

	case SpaceGame::eState::Game:
		m_scene->GetActorByName("Title")->active = false;
		m_gameTimer += dt; // a timer that counts down
		m_spawnTimer += dt;
		//if the player lasts long enough
		if (m_gameTimer >= 30.0f)
		{
			m_state = eState::Winner;
		}
 		if (m_spawnTimer >= m_spawnTime)
		{
			m_spawnTimer = 0;
			auto enemy = INSTANTIATE(kiko::Enemy, "Enemy");
			enemy->transform = kiko::Transform({ kiko::random(kiko::g_renderer.getWidth()),kiko::random(kiko::g_renderer.getHeight()) }, kiko::randomf(kiko::TwoPi), kiko::random(1,2));
			enemy->Initialize();
			m_scene->Add(std::move(enemy));
			
		}

		// Restart the game on 'R' key press
		if (kiko::g_inputSystem.GetKeyDown(SDL_SCANCODE_R))
		{
			kiko::g_audioSystem.PlayOneShot("menu");
			m_state = eState::Title;
			m_scene->RemoveAll();
		}
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
			emitter->lifespan = 1.0f;
			m_scene->Add(std::move(emitter));
		} // emitter stuff
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
	m_winnerText->Create(kiko::g_renderer, "Winner", { 1, 1, 1, 1 });
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
	m_scene->Draw(renderer);

	m_scoreText->Draw(renderer, 40, 40);
	kiko::g_particleSystem.Draw(renderer);
}
void SpaceGame::OnAddPoints(const kiko::Event& event)
{
	m_score += std::get<int>(event.data);
}
void SpaceGame::OnPlayerDead(const kiko::Event& event)
{
	m_lives--;
	m_state = eState::PlayerDeadStart;
}