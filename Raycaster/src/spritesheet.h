#pragma once

#include <vector>
#include <cstdint>

struct Texture
{
	uint8_t* pixels;
	uint32_t width, height;

	const uint8_t* GetColumn(size_t col) const
	{
		return pixels + height * col * 3;
	}
};

class SpriteSheet
{
public:
	SpriteSheet() = default;
	SpriteSheet(const char* filename, int width, int height, bool looped);
	~SpriteSheet();

	inline int GetCountX() const { return m_CountX; }
	inline int GetCountY() const { return m_CountY; }
	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
	inline bool IsLooped() const { return m_Looped; }

	Texture operator[](size_t index) const;
public:
	int m_CountX, m_CountY;
	int m_Width, m_Height;
	int m_TextureWidth, m_TextureHeight; // Width and height of single texture in sprite sheet
	uint8_t* m_Pixels;
	bool m_Looped;
};