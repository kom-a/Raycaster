#pragma once

#include <unordered_map>

#include "utils/resourcemanager.h"
#include "spritesheet.h"

struct AnimationLayer
{
	const SpriteSheet* sheet;
	float scaleFactor;
	int fps;
};

class Animation
{
public:
	Animation();

	void AddLayer(const SpriteSheet* layer, const float& scaleFactor, const int& fps, const std::string& name);
	Texture GetCurrentTexture() const;

	void Play(const std::string& name);
	void Update(const double& deltaTime);

	inline float GetScaleFactor() const { return m_CurrentLayer.scaleFactor; }
	inline bool IsFinished() const { return !m_CurrentLayer.sheet->IsLooped() && m_CurrentFrame == m_CurrentLayer.sheet->GetCountX() - 1; }

private:
	AnimationLayer GetSpriteSheet(const std::string& name);

private:
	std::unordered_map<std::string, AnimationLayer> m_Layers;
	double m_AnimationTimer;
	AnimationLayer m_CurrentLayer;
	int m_CurrentFrame;
};