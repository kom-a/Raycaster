#include <iostream>

#include "resourcemanager.h"

std::unordered_map<std::string, SpriteSheet> ResourceManager::m_SpriteSheets;

void ResourceManager::LoadSpriteSheet(const std::string& filename, int textureWidth, int textureHeight, bool looped, const std::string& name)
{
	m_SpriteSheets[name] = SpriteSheet(filename.c_str(), textureWidth, textureHeight, looped);
}

const SpriteSheet* ResourceManager::GetSpriteSheet(const std::string& name)
{
	if (m_SpriteSheets.find(name) == m_SpriteSheets.end())
	{
		std::cout << "Failed to find \'" << name << "\'" << " resource" << std::endl;
		return 0;
	}

	return &m_SpriteSheets[name];
}
