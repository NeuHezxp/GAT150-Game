#pragma once
#include "Framework/Game.h"
#include "Renderer/Text.h"
#include <memory>

#include "Framework/Event/Event.h"
#include "Framework/Event/EventManager.h"

class CarDerby : public kiko::Game, public kiko::IEventListener
{
public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		PlayerDeadStart,
		PlayerDead,
		GameOverStart,
		GameOver,
		Winner
	};

public:

	// Inherited via Game
	virtual bool Initialize() override;
	virtual bool Shutdown() override;

	virtual void Update(float dt) override;
	virtual void Draw(kiko::Renderer& renderer) override;

	void SetState(eState state) { m_state = state; }

	virtual void OnAddPoints(const kiko::Event& event);
	void OnPlayerDead(const kiko::Event& event);


private:
	eState m_state = eState::Title;
	float m_spawnTimer = 0;
	float m_spawnTime = 10.0f; //3 secs spawn enemy

	float m_stateTimer = 0;
	float m_gameTimer = 0;
	float m_waiting = false;

	std::shared_ptr<kiko::Font> m_font;
	std::unique_ptr<kiko::Text> m_scoreText;
	std::unique_ptr<kiko::Text> m_titleText;
	std::unique_ptr<kiko::Text> m_timerText;
	std::unique_ptr<kiko::Text> m_winnerText;
	std::unique_ptr<kiko::Text> m_gameovertext;
};
