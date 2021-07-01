#pragma once

#include <glm/glm.hpp>
#include "../sprite.h"

class MagicBall
{
public:
	MagicBall(const glm::vec2& position, const glm::vec2& direction, const float& speed)
		: 
		m_Position(position), 
		m_Direction(direction), 
		m_Speed(speed), 
		m_Sprite("res/FlyingEye/magicBall.png", position.x, position.y)
	{

	}

	void Update(double deltaTime)
	{
		m_Position += m_Direction * m_Speed * (float)deltaTime;
		m_Sprite.SetPosition(m_Position);
	}

	inline const glm::vec2& GetPosition() const { return m_Position; }
	inline const Sprite& GetSprite() const { return m_Sprite; }

private:
	glm::vec2 m_Position;
	glm::vec2 m_Direction;
	float m_Speed;
	Sprite m_Sprite;
};
