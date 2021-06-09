#include "renderer.h"

#define _USE_MATH_DEFINES
#include <math.h>

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

#if 0
void Renderer::DrawSprite(const Sprite& sprite, const Player& player)
{
	float sprite_dir = atan2(sprite.GetPosition().y - player.GetPosition().y, sprite.GetPosition().x - player.GetPosition().x);

	while (sprite_dir - player.GetAngle() > (float)M_PI) sprite_dir -= 2 * M_PI;
	while (sprite_dir - player.GetAngle() < (float)-M_PI) sprite_dir += 2 * M_PI;

	float sprite_dist = glm::length(player.GetPosition() - sprite.GetPosition());
	size_t sprite_screen_size = std::min(2000, static_cast<int>(m_Height / sprite_dist));

	// TODO: remove hard coded fov
	const float FOV = (float)M_PI / 3.0f;
	int h_offset = (sprite_dir - player.GetAngle()) * (m_Width / 2) / (FOV) + (m_Width / 2) / 2 - sprite_screen_size / 2.0f;
	int v_offset = m_Height / 2 - sprite_screen_size / 2.0f;

	for (size_t i = 0; i < sprite_screen_size; i++) {
		if (h_offset + int(i) < 0 || h_offset + i >= m_Width / 2) continue;
		for (size_t j = 0; j < sprite_screen_size; j++) {
			if (v_offset + int(j) < 0 || v_offset + j >= m_Height) continue;
			Draw(m_Width / 2 + h_offset + i, v_offset + j, glm::ivec3(0));
		}
	}
}
#endif

void Renderer::DrawSprite(const Sprite& sprite, const Player& player)
{
	glm::vec2 sprite_dir = sprite.GetPosition() - player.GetPosition();
	float sprite_angle = glm::atan(sprite_dir.y, sprite_dir.x);
	const float fov = (float)M_PI / 3.0;

	while (sprite_angle - player.GetAngle() > (float)M_PI) sprite_angle -= 2 * M_PI;
	while (sprite_angle - player.GetAngle() < (float)-M_PI) sprite_angle += 2 * M_PI;

	int x_center_of_sprite_on_screen = static_cast<int>((sprite_angle - player.GetAngle() + fov / 2) * m_Width / fov);
	size_t sprite_screen_size = std::min(1000, static_cast<int>(m_Height / glm::length(sprite_dir)));

	int top_y = m_Height / 2 - sprite_screen_size / 2;
	for (int x = 0; x < sprite_screen_size; x++)
	{
		int x_coord_on_screen = x_center_of_sprite_on_screen - sprite_screen_size / 2 + x;
		if (x_coord_on_screen >= (int)m_Width) break;
		if (x_coord_on_screen < 0) continue;
		const uint8_t* column = sprite[static_cast<int>((float)x / sprite_screen_size * sprite.GetSize().x)];
		for (int y = 0; y < sprite_screen_size; y++)
		{
			int r = column[static_cast<int>((float)y / sprite_screen_size * sprite.GetSize().y) * 3 + 0];
			int g = column[static_cast<int>((float)y / sprite_screen_size * sprite.GetSize().y) * 3 + 1];
			int b = column[static_cast<int>((float)y / sprite_screen_size * sprite.GetSize().y) * 3 + 2];

			if (r == 255 && g == 0 && b == 255) continue;
			Draw(x_coord_on_screen,  top_y + y, glm::ivec3(r, g, b));
		}
	}
}
void Renderer::Flush()
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}