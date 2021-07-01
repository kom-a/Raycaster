#pragma once

#include "enemy.h"
#include "magicball.h"

class FlyingEye : public Enemy
{
public:
	FlyingEye(const glm::vec2& position)
		: Enemy(position), m_ReloadingTime(5.0f), m_Timer(0)
	{
		m_Damage = 10;
		m_Health = 40;

		const SpriteSheet* idleSheet = ResourceManager::GetSpriteSheet("FlyingEyeIdleSheet");
		const SpriteSheet* takeHitSheet = ResourceManager::GetSpriteSheet("FlyingEyeTakeHitSheet");
		const SpriteSheet* deathSheet = ResourceManager::GetSpriteSheet("FlyingEyeDeathSheet");
		const SpriteSheet* attackSheet = ResourceManager::GetSpriteSheet("FlyingEyeAttackSheet");

		m_Animation->AddLayer(idleSheet, 1.0f, 15, "Idle");
		m_Animation->AddLayer(takeHitSheet, 1.0f, 9, "TakeHit");
		m_Animation->AddLayer(deathSheet, 1.0f, 9, "Death");
		m_Animation->AddLayer(attackSheet, 1.0f, 13, "Attack");

		m_Animation->Play("Idle");
	}

	void Update(double deltaTime, const Player& player, const Map& map) override
	{
		Enemy::Update(deltaTime, player, map);
		if (m_State == EnemyState::Death) return;
		m_Timer += deltaTime;

		const Camera* camera = player.GetCamera();
		glm::vec2 playerDirection = player.GetPosition() - m_Position;
		float length = glm::length(playerDirection);
		playerDirection = glm::normalize(playerDirection);

		char tex;
		glm::vec2 intersection = camera->CastRay(player.GetPosition(), -playerDirection, map, &tex);
		float wallDistance = glm::length(intersection - player.GetPosition());
		bool isPlayerVisible = length < wallDistance ? true : false;

		const float attackDistance = 13.0f;
		const float chaseDistance = 20.0f;

		if (m_State == EnemyState::TakeHit && m_Animation->IsFinished())
		{
			m_Animation->Play("Idle");
			m_State = EnemyState::Idle;
		}
		if (isPlayerVisible && m_State != EnemyState::Attack && m_Timer >= m_ReloadingTime)
		{
			if (length < attackDistance)
			{
				m_State = EnemyState::Attack;
				m_Animation->Play("Attack");
				m_Bullets.push_back(new MagicBall(m_Position, playerDirection, 5.0f));
			}
		}
		if (length < chaseDistance && length > attackDistance)
		{
			float speed = 1.5f * deltaTime;
			m_Position += playerDirection * speed;
		}
		if (m_State == EnemyState::Attack && m_Animation->IsFinished())
		{
			m_State = EnemyState::Idle;
			m_Animation->Play("Idle");
		}
		if (m_Timer >= m_ReloadingTime)
			m_Timer = 0;
		
		for (MagicBall* m : m_Bullets)
		{
			m->Update(deltaTime);
		}
	}

	void TakeHit(int damage) override
	{
		Enemy::TakeHit(damage);
		if (m_Health == 0)
		{
			m_Bullets.clear();
		}
	}

	inline std::vector<MagicBall*> GetBullets() const { return m_Bullets; }



private:
	float m_ReloadingTime;
	float m_Timer;
	std::vector<MagicBall*> m_Bullets;
};