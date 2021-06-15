#pragma once

#include <unordered_map>

#include "utils/resourcemanager.h"
#include "spritesheet.h"

class Animation
{
public:
	Animation();

	void AddLayer(const SpriteSheet* layer, const std::string& name);
private:
	std::unordered_map<std::string, const SpriteSheet*> m_Layers;
};