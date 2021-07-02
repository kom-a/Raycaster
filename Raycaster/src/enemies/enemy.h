#pragma once

#include <glm/glm.hpp>
#include "../spritesheet.h"
#include "../utils/resourcemanager.h"
#include "../animation.h"
#include "../input/input.h"
#include "../map.h"

#include <iostream>

class Player;

enum class EnemyState
{
	Idle,
	Run,
	Attack,
	TakeHit,
	Death
};

class Enemy
{
protected:
	Enemy(const glm::vec2& position)
		: m_Position(position), m_Health(100), m_Damage(1), m_State(EnemyState::Idle)
	{
		m_Animation = new Animation();
	}

public:
	virtual ~Enemy()
	{
		delete m_Animation;
	}

public:
	inline const glm::vec2& GetPosition() const { return m_Position; }
	inline Texture GetCurrentTexture() const { return m_Animation->GetCurrentTexture(); }
	inline const float& GetScaleFactor() const { return m_Animation->GetScaleFactor(); }
	inline Animation* GetAnimation() const { return m_Animation; }
	inline EnemyState GetState() const { return m_State; }

	virtual void Update(double deltaTime, Player& player, const Map& map)
	{
		m_Animation->Update(deltaTime);
	};

	virtual void TakeHit(int damage)
	{
		m_Animation->Play("TakeHit");
		m_State = EnemyState::TakeHit;
		m_Health -= damage;
		if (m_Health <= 0)
		{
			m_Health = 0;
			m_Animation->Play("Death");
			m_State = EnemyState::Death;
		}
	}

protected:
	Animation* m_Animation;
	glm::vec2 m_Position;

	int m_Health;
	int m_Damage;
	EnemyState m_State;
};
