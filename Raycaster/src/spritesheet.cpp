#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "utils/filemanager.h"

#include "spritesheet.h"

SpriteSheet::SpriteSheet(const char* filename, int width, int height, bool looped)
	: m_TextureWidth(width), m_TextureHeight(height), m_Looped(looped)
{
	int comp;
	uint8_t* rowMajorPixels = stbi_load(filename, &m_Width, &m_Height, &comp, 3);
	if (!rowMajorPixels)
	{
		std::cout << "Failed to load texture \'" << filename << "\'" << std::endl;
		return;
	}

	if (m_Width % m_TextureWidth != 0 || m_Height % m_TextureHeight != 0)
	{
		std::cout << "Incorrect sprite sheet format: \'" << filename << "\'" << std::endl;
		return;
	}
	m_CountX = m_Width / m_TextureWidth;
	m_CountY = m_Height / m_TextureHeight;

	m_Pixels = FileManager::ConvertToColumnMajor(rowMajorPixels, m_Width, m_Height, 3);
	STBI_FREE(rowMajorPixels);
}

SpriteSheet::~SpriteSheet()
{
	// delete[] m_Pixels;
}

Texture SpriteSheet::operator[](size_t index) const
{
	Texture texture;
	texture.width = m_TextureWidth;
	texture.height = m_TextureHeight;
	texture.pixels = m_Pixels + (m_TextureWidth * m_TextureHeight * 3) * index;
	return texture;
}
