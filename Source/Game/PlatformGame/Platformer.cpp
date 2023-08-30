
#include "Platformer.h"



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
bool Platformer::Initialize()
{
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
	m_scene->Load("scenes/tilemap.json");
	m_scene->Load("scenes/CarScene.json");
	m_scene->Initialize();
	EVENT_SUBSCRIBE("OnAddPoints", Platformer::OnAddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", Platformer::OnPlayerDead);

	return true;
}

bool Platformer::Shutdown()
{
	return false;
}

void Platformer::Update(float dt)
{
	switch (m_state)
	{
	case Platformer::eState::Title:
	
		//reset score/timer
		m_score = 0;
		m_gameTimer = 0.0f;
		break;
	case Platformer::eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_state = eState::StartLevel;
		break;

	case Platformer::eState::StartLevel:
		m_scene->RemoveAll();

		{
			//Create Player

			/*auto player = INSTANTIATE(kiko::Player, "Player");
			player->transform = kiko::Transform({ 400,300 }, 0, 1);
			player->Initialize();
			m_scene->Add(std::move(player));


			m_scene->GetActorByName("Title")->active = true;*/

		}
		m_state = eState::Game;
		break;

	case Platformer::eState::Game:
		
		break;
	case eState::PlayerDeadStart:

		break;
	case Platformer::eState::PlayerDead:
		
		break;
	case Platformer::eState::GameOver:
		
		break;
	case Platformer::eState::Winner:
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
	m_scene->Update(dt);
}

void Platformer::Draw(kiko::Renderer& renderer)
{
	m_scene->Draw(renderer);
}


void Platformer::OnAddPoints(const kiko::Event& event)
{
	m_score += std::get<int>(event.data);
}
void Platformer::OnPlayerDead(const kiko::Event& event)
{
	m_lives--;
	m_state = eState::PlayerDeadStart;
}