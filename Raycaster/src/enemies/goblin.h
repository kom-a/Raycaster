#pragma once

#include "enemy.h"

class Goblin : public Enemy
{
public:
	Goblin()
	{

	}

	Goblin(const glm::vec2& position)
		: Enemy(position, "res/goblin.png", 33, 36)
	{
		m_CurrentFrame = 0;
		m_Damage = 10;
		m_Health = 100;
		m_ScaleFactor = 0.45f;
	}
};