#pragma once

#include "Sprite.h"

class Player : public Sprite
{
private:
	float m_rot;
	float m_speed;
	float m_topSpeed;
public:
	Player();
	Player(float topSpeed);
	float GetRot();
	void SetRot(float rot);
	void IncRot(float rot);
	float GetSpeed();
	void SetSpeed(float speed);
	void IncSpeed(float speed);
	float GetTopSpeed();
	void SetTopSpeed(float speed);
};

