#pragma once

#include <unordered_map>

#include "utils/resourcemanager.h"
#include "spritesheet.h"

class Animation
{
public:
	Animation();

	void AddLayer(const SpriteSheet* layer, const int& fps, const std::string& name);
	Texture GetCurrentTexture() const;

	void Play(const std::string& name);
	void Update(const double& deltaTime);

private:
	const SpriteSheet* GetSpriteSheet(const std::string& name);

private:
	std::unordered_map<std::string, const SpriteSheet*> m_Layers;
	double m_AnimationTimer;
	int m_FPS;
	const SpriteSheet* m_CurrentSpriteSheet;
	int m_CurrentFrame;
};