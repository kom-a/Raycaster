#pragma once

#include "enemy.h"
#include "utils/resourcemanager.h"

class Goblin : public Enemy
{
public:
	Goblin(const glm::vec2& position)
		 : Enemy(position, "GoblinIdleSheet")
	{
		m_Damage = 10;
		m_Health = 100;
		m_ScaleFactor = 0.75f;

		const SpriteSheet* attackSheet = ResourceManager::GetSpriteSheet("GoblinAttackSheet");
		const SpriteSheet* deathSheet = ResourceManager::GetSpriteSheet("GoblinDeathSheet");
		const SpriteSheet* takeHitSheet = ResourceManager::GetSpriteSheet("GoblinTakeHitSheet");

		m_Animation->AddLayer(attackSheet, 5, "Attack");
		m_Animation->AddLayer(deathSheet, 5, "Death");
		m_Animation->AddLayer(takeHitSheet, 15, "TakeHit");

		m_Animation->Play("Idle");
	}
};