#include <iostream>

#include "animation.h"

Animation::Animation()
	: m_AnimationTimer(0), 
	m_CurrentFrame(0), 
	m_FPS(0), 
	m_CurrentSpriteSheet(nullptr)
{
	m_Layers = std::unordered_map<std::string, const SpriteSheet*>();
}

void Animation::AddLayer(const SpriteSheet* layer, const int& fps, const std::string& name)
{
	m_Layers[name] = layer;
	m_FPS = fps;
}

Texture Animation::GetCurrentTexture() const
{
	return m_CurrentSpriteSheet->operator[](m_CurrentFrame);
}

void Animation::Play(const std::string& name)
{
	m_CurrentSpriteSheet = GetSpriteSheet(name);
	m_CurrentFrame = 0;
}

void Animation::Update(const double& deltaTime)
{
	m_AnimationTimer += deltaTime;
	if (m_AnimationTimer >= 1.0 / m_FPS)
	{
		m_CurrentFrame++;

		if (m_CurrentFrame >= m_CurrentSpriteSheet->GetCountX())
		{
			if (m_CurrentSpriteSheet->IsLooped())
				m_CurrentFrame = 0;
			else
			{
				// m_CurrentFrame = m_CurrentSpriteSheet->GetCountX() - 1; // Last Frame
				Play("Idle");
			}
		}

		m_AnimationTimer = 0;
	}
}

const SpriteSheet* Animation::GetSpriteSheet(const std::string& name)
{
	if (m_Layers.find(name) == m_Layers.end())
	{
		std::cout << "Failed to find \'" << name << "\'" << std::endl;
		return 0;
	}

	return m_Layers[name];
}
