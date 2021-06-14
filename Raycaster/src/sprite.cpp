#include <iostream>

#define STBI_MALLOC
#define STBI_REALLOC
#define STBI_FREE

#include "sprite.h"
#include "utils/filemanager.h"
#include "stb_image.h"

Sprite::Sprite(const char* filename, int x, int y)
	: m_Pixels(nullptr), m_Position(x, y)
{
	int comp;
	uint8_t* rowMajorPixels = stbi_load(filename, &m_Size.x, &m_Size.y, &comp, 3);
	if (!rowMajorPixels)
	{
		std::cout << "Failed to load sprite \'" << filename << "\'" << std::endl;
		return;
	}

	m_Pixels = FileManager::ConvertToColumnMajor(rowMajorPixels, m_Size.x, m_Size.y, 3);
	STBI_FREE(rowMajorPixels);
}

Sprite::~Sprite()
{
	delete[] m_Pixels;
}

const uint8_t* Sprite::operator[](size_t index) const
{
	return m_Pixels + m_Size.y * index * 3;
}
