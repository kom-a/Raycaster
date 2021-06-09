#pragma once

#include "glm/glm.hpp"
#include "shader.h"
#include "sprite.h"
#include "player.h"

class Renderer
{
public:
	Renderer(uint32_t width, uint32_t height);
	~Renderer();

	void Clear();
	void Draw(int x, int y, const glm::ivec3& color);
	void DrawSprite(const Sprite& sprite, const Player& player);
	void Flush();

private:
	uint32_t m_Width, m_Height;
	uint32_t m_VAO;
	uint32_t m_VBO;
	uint32_t m_Texture;
	uint8_t* m_Buffer;
	size_t m_BufferSize;
	Shader* m_Shader;
};