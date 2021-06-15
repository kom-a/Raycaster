#pragma once

#include "enemy.h"

class Goblin : public Enemy
{
public:
	Goblin()
	{

	}

	Goblin(const glm::vec2& position)
		 : Enemy(position, "GoblinIdleSheet")
		 // : Enemy(position, "res/Goblin/Attack.png", 90, 48)
	{
		m_CurrentFrame = 0;
		m_Damage = 10;
		m_Health = 100;
		m_ScaleFactor = 0.75f;
	}
};