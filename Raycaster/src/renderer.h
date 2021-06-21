#pragma once

#include "glm/glm.hpp"
#include "shader.h"
#include "sprite.h"
#include "player.h"
#include "spritesheet.h"
#include "enemies/enemy.h"

class Renderer
{
public:
	Renderer(uint32_t width, uint32_t height);
	~Renderer();

	void Clear(const int& horizontOffset);
	void Draw(int x, int y, const glm::ivec3& color);
	void DrawWall(int x, float distance, const Texture& texture, int column, const Player& player);
	void DrawSky(const Texture& texture, const Player& player);
	void DrawSprite(const Sprite& sprite, const Player& player);
	void DrawPlayer(const Player& player, int x_offset, int y_offset);
	void DrawEnemy(const Enemy& enemy, const Player& player);
	void Flush();

private:
	uint32_t m_Width, m_Height;
	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_Texture;
	uint8_t* m_Buffer;
	size_t m_BufferSize;
	Shader* m_Shader;
	float* m_DepthBuffer;
};