#pragma once

#include <glm/glm.hpp>
#include "../spritesheet.h"

class Enemy
{
protected:
	Enemy()
	{

	}

	Enemy(const glm::vec2& position, const char* spriteSheetFilename, int textureWidth, int textureHeight)
		: m_Position(position), m_CurrentFrame(0), m_Health(0), m_Damage(0), m_AnimationTimer(0), m_ScaleFactor(1)
	{
		m_SpriteSheet = new SpriteSheet(spriteSheetFilename, textureWidth, textureHeight);
	}

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
		m_AnimationTimer += deltaTime;
		if (m_AnimationTimer >= 0.1)
		{
			m_CurrentFrame++;
			if (m_CurrentFrame >= m_SpriteSheet->GetCountX())
			{
				m_CurrentFrame = 0;
			}
			m_AnimationTimer = 0;
		}
	};



protected:
	double m_AnimationTimer;
	int m_CurrentFrame;
	SpriteSheet* m_SpriteSheet;
	float m_ScaleFactor;

	glm::vec2 m_Position;

	int m_Health;
	int m_Damage;
};
