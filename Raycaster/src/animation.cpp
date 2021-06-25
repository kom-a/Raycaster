#include <iostream>

#include "animation.h"
#include <assert.h>

Animation::Animation()
	: m_AnimationTimer(0), 
	m_CurrentFrame(0)
{
	m_Layers = std::unordered_map<std::string, AnimationLayer>();
}

void Animation::AddLayer(const SpriteSheet* layer, const float& scaleFactor, const int& fps, const std::string& name)
{
	AnimationLayer animationLayer;
	animationLayer.sheet = layer;
	animationLayer.scaleFactor = scaleFactor;
	animationLayer.fps = fps;
	m_Layers[name] = animationLayer;
}

Texture Animation::GetCurrentTexture() const
{
	return m_CurrentLayer.sheet->operator[](m_CurrentFrame);
}

void Animation::Play(const std::string& name)
{
	m_CurrentLayer = GetSpriteSheet(name);
	m_CurrentFrame = 0;
}

void Animation::Update(const double& deltaTime)
{
	m_AnimationTimer += deltaTime;
	if (m_AnimationTimer >= 1.0 / m_CurrentLayer.fps)
	{
		m_CurrentFrame++;

		if (m_CurrentFrame >= m_CurrentLayer.sheet->GetCountX())
		{
			if (m_CurrentLayer.sheet->IsLooped())
				m_CurrentFrame = 0;
			else 
			{
				if(m_CurrentLayer.sheet != m_Layers["Death"].sheet)
					Play("Idle");
				else
					m_CurrentFrame = m_CurrentLayer.sheet->GetCountX() - 1; // Last Frame
			}
		}

		m_AnimationTimer = 0;
	}
}

AnimationLayer Animation::GetSpriteSheet(const std::string& name)
{
	if (m_Layers.find(name) == m_Layers.end())
	{
		std::cout << "Failed to find \'" << name << "\'" << std::endl;
		assert(false);
	}

	return m_Layers[name];
}
