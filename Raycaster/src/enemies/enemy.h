#pragma once

#include <glm/glm.hpp>
#include "../spritesheet.h"
#include "utils/resourcemanager.h"

#include "../input/input.h"

class Enemy
{
protected:
	Enemy()
	{

	}

	Enemy(const glm::vec2& position, const char* recourceName)
		: m_Position(position), m_CurrentFrame(0), m_Health(0), m_Damage(0), m_AnimationTimer(0), m_ScaleFactor(1)
	{
		m_SpriteSheet = ResourceManager::GetSpriteSheet(recourceName);
	}

public:
	virtual ~Enemy()
	{
		// delete m_SpriteSheet;
	}

public:
	inline const glm::vec2& GetPosition() const { return m_Position; }
	inline const Texture GetCurrentTexture() const { return m_SpriteSheet->operator[](m_CurrentFrame); }
	inline const float& GetScaleFactor() const { return m_ScaleFactor; }

	virtual void Update(double deltaTime)
	{
		if (Keyboard::IsKeyPressed('R'))
			m_SpriteSheet = ResourceManager::GetSpriteSheet("GoblinTakeHitSheet");
		else
			m_SpriteSheet = ResourceManager::GetSpriteSheet("GoblinIdleSheet");

		m_AnimationTimer += deltaTime;
		if (m_AnimationTimer >= 0.1)
		{
			m_CurrentFrame++;

			if (m_CurrentFrame >= m_SpriteSheet->GetCountX() || m_CurrentFrame <= 0)
			{
				m_CurrentFrame = 0;
			}
			m_AnimationTimer = 0;
		}

		// m_Position += 0.1f * deltaTime;
	};



protected:
	double m_AnimationTimer;
	int m_CurrentFrame;
	const SpriteSheet* m_SpriteSheet;
	float m_ScaleFactor;

	glm::vec2 m_Position;

	int m_Health;
	int m_Damage;

};
