#pragma once

#include "glm/glm.hpp"
#include "shader.h"

class Renderer
{
public:
	Renderer(uint32_t width, uint32_t height);
	~Renderer();

	void Clear();
	void FillRect(int x, int y, int width, int height, const glm::ivec3& color);
	void Draw(int x, int y, const glm::ivec3& color);
	void DrawWall(int x, int height, const glm::ivec3& color);
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