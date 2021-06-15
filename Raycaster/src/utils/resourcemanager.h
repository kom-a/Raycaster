#pragma once

#include <unordered_map>

#include "../spritesheet.h"

class ResourceManager
{
public:

	static void LoadSpriteSheet(const std::string& filename, int textureWidth, int textureHeight, bool looped, const std::string& name);
	static const SpriteSheet* GetSpriteSheet(const std::string& name);
	
	static std::unordered_map<std::string, SpriteSheet> m_SpriteSheets;
private:

};