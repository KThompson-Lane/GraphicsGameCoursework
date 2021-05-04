#include "Player.h"

Player::Player()
{
	m_speed = 0.0f;
	m_rot = 0.0f;
	m_topSpeed = 2.50f;
}

Player::Player(float topSpeed)
{
	m_speed = 0.0f;
	m_rot = 0.0f;
	m_topSpeed = topSpeed;
}

float Player::GetRot()
{
	return m_rot;
}
void Player::SetRot(float rot)
{
	m_rot = rot;
}
void Player::IncRot(float rot)
{
	if (m_rot + rot <= 0)
	{
		m_rot = 360*3.14/180;
	}
	m_rot = fmod(m_rot + rot, 360*3.14/180);
}

float Player::GetSpeed() 
{
	return m_speed;
}

void Player::SetSpeed(float speed)
{
	m_speed = speed;
}

void Player::IncSpeed(float speed)
{
	m_speed += speed;
}

float Player::GetTopSpeed()
{
	return m_topSpeed;
}

void Player::SetTopSpeed(float speed)
{
	m_topSpeed = speed;
}


