#include "renderer.h"

Renderer::Renderer(uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height)
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	int error = glGetError();

	m_BufferSize = width * height * 3;
	m_Buffer = new uint8_t[m_BufferSize];
	memset(m_Buffer, 0x00, m_BufferSize);

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenBuffers(1, &m_VBO);

	error = glGetError();

	float vertices[] =
	{
		-1.0f, -1.0f, 0.0f,		0.0f, 1.0f,
		-1.0f,  1.0f, 0.0f,		0.0f, 0.0f,
		 1.0f,  1.0f, 0.0f,		1.0f, 0.0f,

		 1.0f,  1.0f, 0.0f,		1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,		1.0f, 1.0f,
		-1.0f, -1.0f, 0.0f,		0.0f, 1.0f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(0 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	m_Shader = new Shader("res/vertex.glsl", "res/fragment.glsl");
	m_Shader->Bind();
	error = glGetError();
}

Renderer::~Renderer()
{
	delete[] m_Buffer;
	delete m_Shader;
}

void Renderer::Clear()
{
	memset(m_Buffer, 200, m_BufferSize);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::FillRect(int x, int y, int width, int height, const glm::ivec3& color)
{
	for (int i = y; i < y + height; i++)
	{
		for (int j = x; j < x + width; j++)
		{
			if (i >= 0 && i < m_Height && j >= 0 && j < m_Width)
			{
				Draw(j, i, color);
			}
		}
	}
}

void Renderer::Draw(int x, int y, const glm::ivec3& color)
{
	int index = (y * m_Width + x) * 3;
	if (index >= 0 && index < m_BufferSize)
	{
		m_Buffer[index + 0] = color.r;
		m_Buffer[index + 1] = color.g;
		m_Buffer[index + 2] = color.b;
	}
	
}

void Renderer::DrawWall(int x, int height, const glm::ivec3& color)
{
	int top = m_Height / 2 - height / 2;
	for (int i = 0; i < height; i++)
		Draw(x, top + i, color);
	
}

void Renderer::Flush()
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}