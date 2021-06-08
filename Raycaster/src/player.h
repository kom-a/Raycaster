#pragma once

#include <glm/glm.hpp>

#include "camera.h"

class Player
{
public:
	Player(float x, float y, float angle);
	~Player();

	void Update(const double& deltaTime);
	inline int GetYOffset() const { return m_YOffset; }

public:
	glm::vec2 m_Position;
	float m_Angle;
	float m_Speed;
	Camera* m_Camera;

private:
	double m_LastMouseX, m_LastMouseY;
	int m_YOffset;

};