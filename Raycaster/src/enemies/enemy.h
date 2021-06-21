#pragma once

#include <glm/glm.hpp>
#include "../spritesheet.h"
#include "utils/resourcemanager.h"
#include "animation.h"
#include "../input/input.h"

class Enemy
{
protected:
	Enemy(const glm::vec2& position, const char* recourceName)
		: m_Position(position), m_Health(0), m_Damage(0), m_ScaleFactor(1)
	{
		m_SpriteSheet = ResourceManager::GetSpriteSheet(recourceName);
		m_Animation = new Animation();
		m_Animation->AddLayer(m_SpriteSheet, 0000000000, "Idle");
	}

public:
	virtual ~Enemy()
	{
		 //delete m_SpriteSheet;
	}

public:
	inline const glm::vec2& GetPosition() const { return m_Position; }
	inline Texture GetCurrentTexture() const { return m_Animation->GetCurrentTexture(); }
	inline const float& GetScaleFactor() const { return m_ScaleFactor; }
	inline Animation* GetAnimation() const { return m_Animation; }


	virtual void Update(double deltaTime)
	{
		m_Animation->Update(deltaTime);
	};

protected:
	Animation* m_Animation;
	const SpriteSheet* m_SpriteSheet;
	float m_ScaleFactor;

	glm::vec2 m_Position;

	int m_Health;
	int m_Damage;
};
