#pragma once
#include "Framework/Actor.h"

class Enemy : public kiko::Actor
{
public:
	Enemy(float speed, float turnRate, const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		Actor{ transform, model },
		m_speed{ speed },
		m_turnRate{ turnRate }//without it calls the default constructor
	{
		m_firerate = 2.0f;
		m_fireTimer = m_firerate;
	}

	void Update(float dt) override; //dt for delta time
	void OnCollision(Actor* other) override;

protected:
	float m_speed = 0;
	float m_turnRate = 0;

	float m_firerate = 0;
	float m_fireTimer = 0;
	float m_health = 25;
};
