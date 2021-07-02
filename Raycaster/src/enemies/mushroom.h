#pragma once

#include <iostream>

#include "enemy.h"
#include "../utils/resourcemanager.h"
#include "../player.h"

#include <glm/glm.hpp>


class Mushroom : public Enemy
{
public:
	Mushroom(const glm::vec2& position)
		: Enemy(position)
	{
		m_Damage = 35;
		m_Health = 80;

		const SpriteSheet* idleSheet = ResourceManager::GetSpriteSheet("MushroomIdleSheet");
		const SpriteSheet* runSheet = ResourceManager::GetSpriteSheet("MushroomRunSheet");
		const SpriteSheet* takeHitSheet = ResourceManager::GetSpriteSheet("MushroomTakeHitSheet");
		const SpriteSheet* deathSheet = ResourceManager::GetSpriteSheet("MushroomDeathSheet");
		const SpriteSheet* attackSheet = ResourceManager::GetSpriteSheet("MushroomAttackSheet");

		m_Animation->AddLayer(idleSheet, 0.7f, 10, "Idle");
		m_Animation->AddLayer(runSheet, 0.7f, 10, "Run");
		m_Animation->AddLayer(takeHitSheet, 0.9f, 18, "TakeHit");
		m_Animation->AddLayer(deathSheet, 0.7f, 10, "Death");
		m_Animation->AddLayer(attackSheet, 0.80f, 10, "Attack");

		m_Animation->Play("Idle");
	}

	void Update(double deltaTime, Player& player, const Map& map) override
	{
		Enemy::Update(deltaTime, player, map);
		if (m_State == EnemyState::Death) return;
		const Camera* camera = player.GetCamera();

		const float chaseDistance = 8.0f;
		const float attackDistance = 0.6f;

		glm::vec2 moveDirection = player.GetPosition() - m_Position;
		float length = glm::length(moveDirection);
		moveDirection = glm::normalize(moveDirection);

		char tex;
		glm::vec2 intersection = camera->CastRay(player.GetPosition(), -moveDirection, map, &tex);
		float wallDistance = glm::length(intersection - player.GetPosition());
		bool isPlayerVisible = length < wallDistance ? true : false;

		if (length > chaseDistance && m_State == EnemyState::Run || !isPlayerVisible && m_State == EnemyState::Run)
		{
			m_Animation->Play("Idle");
			m_State = EnemyState::Idle;
		}
		else if (length < chaseDistance && m_State == EnemyState::Idle && isPlayerVisible)
		{
			m_Animation->Play("Run");
			m_State = EnemyState::Run;
		}
		else if (m_State == EnemyState::Run)
		{
			if (length > attackDistance)
			{
				float speed = 1.5f * deltaTime;
				m_Position += moveDirection * speed;
			}
			else if (m_State != EnemyState::Attack)
			{
				m_State = EnemyState::Attack;
				m_Animation->Play("Attack");
			}
		}
		else if (m_State == EnemyState::TakeHit)
		{
			if (m_Animation->IsFinished())
				m_State = EnemyState::Idle;
		}
		else if (m_State == EnemyState::Attack)
		{
			if (m_Animation->IsFinished())
			{
				m_State = EnemyState::Idle;
				if (length <= attackDistance * 2)
				{
					player.TakeHit(m_Damage);
				}
			}
		}
	}

	void TakeHit(int damage) override
	{
		Enemy::TakeHit(damage);
	}
};