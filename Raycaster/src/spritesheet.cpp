#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "spritesheet.h"

SpriteSheet::SpriteSheet(const char* filename, int width, int height)
	: m_TextureWidth(width), m_TextureHeight(height)
{
	int comp;
	uint8_t* rowMajorPixels = stbi_load(filename, &m_Width, &m_Height, &comp, 4);
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

	// Convert pixels to column major order
	m_Pixels = new uint8_t[m_Width * m_Height * 4];
	int i = 0;

	for (int x = 0; x < m_Width; x++)
	{
		for (int y = 0; y < m_Height; y++)
		{
			m_Pixels[i * 4 + 0] = rowMajorPixels[(y * m_Width + x) * 4 + 0];
			m_Pixels[i * 4 + 1] = rowMajorPixels[(y * m_Width + x) * 4 + 1];
			m_Pixels[i * 4 + 2] = rowMajorPixels[(y * m_Width + x) * 4 + 2];
			m_Pixels[i * 4 + 3] = rowMajorPixels[(y * m_Width + x) * 4 + 3];
			i++;
		}
	}
	STBI_FREE(rowMajorPixels);
}

SpriteSheet::~SpriteSheet()
{
	delete[] m_Pixels;
}

Texture SpriteSheet::operator[](size_t index)
{
	Texture texture;
	texture.width = m_TextureWidth;
	texture.height = m_TextureHeight;
	texture.pixels = m_Pixels + (m_TextureWidth * m_TextureHeight * 4) * index;
	return texture;
}
