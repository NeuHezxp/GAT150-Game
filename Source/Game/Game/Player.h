#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"

class Player : public kiko::Actor
{
public:
	Player(float speed, float turnRate, const kiko::Transform& transform) :
		Actor{ transform },
		m_speed{ speed },
		m_turnRate{ turnRate }//without it calls the default constructor
	{}

	void OnCollision(Actor* other) override;
	bool Initialize() override;
	void Update(float dt) override; //dt for delta time
private:
	kiko::PhysicsComponent* m_physicsComponent = nullptr;

protected:
	bool m_isDashing = false;
	float m_dashDuration = 0.2f; // dash's duration
	float m_dashSpeed = 100.0f; // dash's speed
	float m_speed = 0;
	float m_turnRate = 0;
	int m_health = 100;
	kiko::vec2 m_dashDirection; // Dash direction vector
};
