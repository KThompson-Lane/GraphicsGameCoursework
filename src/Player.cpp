#include "Player.h"

Player::Player(float topSpeed)
{
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
	m_rot += rot;
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
