#pragma once

#include "enemy.h"

class FlyingEye : public Enemy
{
public:
	FlyingEye()
	{

	}

	FlyingEye(const glm::vec2& position)
		: Enemy(position, "GoblinIdleSheet")
	{
		m_CurrentFrame = 0;
		m_Damage = 10;
		m_Health = 100;
		m_ScaleFactor = 0.8f;
	}
};