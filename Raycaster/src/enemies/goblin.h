#pragma once

#include <iostream>

#include "enemy.h"
#include "utils/resourcemanager.h"

#include <glm/glm.hpp>


class Goblin : public Enemy
{
public:
	Goblin(const glm::vec2& position)
		 : Enemy(position)
	{
		m_Damage = 10;
		m_Health = 30;
		m_ScaleFactor = 0.7f;

		const SpriteSheet* idleSheet = ResourceManager::GetSpriteSheet("GoblinIdleSheet");
		const SpriteSheet* runSheet = ResourceManager::GetSpriteSheet("GoblinRunSheet");
		const SpriteSheet* attackSheet = ResourceManager::GetSpriteSheet("GoblinAttackSheet");
		const SpriteSheet* deathSheet = ResourceManager::GetSpriteSheet("GoblinDeathSheet");
		const SpriteSheet* takeHitSheet = ResourceManager::GetSpriteSheet("GoblinTakeHitSheet");

		m_Animation->AddLayer(idleSheet,	 0.7f, 10, "Idle");
		m_Animation->AddLayer(runSheet,		 0.7f, 10, "Run");
		m_Animation->AddLayer(attackSheet,	 0.9f, 20, "Attack");
		m_Animation->AddLayer(deathSheet,	 0.7f, 10, "Death");
		m_Animation->AddLayer(takeHitSheet,	 0.7f, 12, "TakeHit");

		m_Animation->Play("Idle");
	}

	void Update(double deltaTime, const Player& player) override
	{
		Enemy::Update(deltaTime, player);
		if (m_State == EnemyState::Death) return;

		const float chaseDistance = 8.0f;
		const float attackDistance = 0.6f;
		
		glm::vec2 moveDirection = player.GetPosition() - m_Position;
		float length = glm::length(moveDirection);


		if (length < chaseDistance && m_State == EnemyState::Idle)
		{
			m_Animation->Play("Run");
			m_State = EnemyState::Run;
		}
		else if(length > chaseDistance && m_State == EnemyState::Run)
		{
			m_Animation->Play("Idle");
			m_State = EnemyState::Idle;
		}
		else if (m_State == EnemyState::Run)
		{
			if (length > attackDistance)
			{
				moveDirection /= length; // normalize
				float speed = 1.0f * deltaTime;
				m_Position += moveDirection * speed;
			}
			else if(m_State != EnemyState::Attack)
			{
				m_State = EnemyState::Attack;
				m_Animation->Play("Attack");
			}
		}
		else if (m_State == EnemyState::TakeHit)
		{
			if(m_Animation->IsFinished())
				m_State = EnemyState::Idle;
		}
		else if (m_State == EnemyState::Attack)
		{
			if (m_Animation->IsFinished())
				m_State = EnemyState::Idle;
		}
	}

	void TakeHit(int damage) override
	{
		Enemy::TakeHit(damage);
		m_ScaleFactor = 0.9f;
	}
};