#pragma once

#include <glm/glm.hpp>

#include "camera.h"

class Player
{
public:
	Player(float x, float y, float angle);
	~Player();

	void Update(const double& deltaTime);

	inline const glm::vec2& GetPosition() const { return m_Position; }
	inline const float& GetAngle() const { return m_Angle; }
	inline int GetYOffset() const { return m_YOffset; }


private:
	glm::vec2 m_Position;
	float m_Angle;
	float m_Speed;

	double m_LastMouseX, m_LastMouseY;
	int m_YOffset;

};
