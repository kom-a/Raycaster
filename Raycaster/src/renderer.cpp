#include "renderer.h"

#define _USE_MATH_DEFINES
#include <math.h>

Renderer::Renderer(uint32_t width, uint32_t height)
	: m_Width(width), m_Height(height)
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	m_BufferSize = width * height * 3;
	m_Buffer = new uint8_t[m_BufferSize];
	memset(m_Buffer, 0x00, m_BufferSize);

	m_DepthBuffer = new float[m_Width * m_Height];

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	glGenBuffers(1, &m_VBO);

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
}

Renderer::~Renderer()
{
	delete[] m_DepthBuffer;
	delete[] m_Buffer;
	delete m_Shader;
}

void Renderer::Clear(const int& horizontOffset)
{
	int skyHeight = m_Height / 2 + horizontOffset;
	if (skyHeight < 0) skyHeight = 0;
	if (skyHeight > (int)(m_Height) - 1) skyHeight = m_Height - 1;
	int groundHeight = m_Height - skyHeight;

	memset(m_Buffer, 200, m_Width * skyHeight * 3);
	memset(m_Buffer + m_Width * skyHeight * 3, 100, m_Width * groundHeight * 3);
	for (size_t i = 0; i < m_Width; i++)
		m_DepthBuffer[i] = 9999999.0f;

	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::Draw(int x, int y, const glm::ivec3& color)
{
	glm::ivec3 clampedColor = glm::clamp(color, glm::ivec3(0), glm::ivec3(255));
	size_t index = (y * m_Width + x) * 3;
	if (index >= 0 && index < m_BufferSize)
	{
		m_Buffer[index + 0] = clampedColor.r;
		m_Buffer[index + 1] = clampedColor.g;
		m_Buffer[index + 2] = clampedColor.b;
		
	}
}

void Renderer::DrawWall(int x, float distance, const Texture& texture, int col, const Player& player)
{
	const float fov = (float)M_PI / 3.0f;
	float angle = (player.GetAngle() - fov / 2.0f) + (float)x / m_Width * fov;
	uint32_t wallHeight = (uint32_t)(m_Height / (distance * glm::cos(angle - player.GetAngle())));
	int top = m_Height / 2 - wallHeight / 2;
	const uint8_t* column = texture.GetColumn(col);
	const float shadowDistance = 50;

	if (wallHeight <= m_Height)
	{
		for (size_t y = 0; y < wallHeight; y++)
		{
			int index = static_cast<int>((float)y / wallHeight * texture.height);
			int r = column[index * 3 + 0];
			int g = column[index * 3 + 1];
			int b = column[index * 3 + 2];

			Draw(x, top + y, glm::vec3(r, g, b) * (1.0f - distance / shadowDistance));
		}
	}
	else 
	{
		// TODO: optimize this
		for (size_t y = 0; y < wallHeight; y++)
		{
			int index = static_cast<int>((float)y / wallHeight * texture.height);
			int r = column[index * 3 + 0];
			int g = column[index * 3 + 1];
			int b = column[index * 3 + 2];

			Draw(x, top + y, glm::ivec3(r, g, b));
		}
	}
	

	if (!std::isnan(distance))
		m_DepthBuffer[x] = distance;
	else
		m_DepthBuffer[x] = 0.001f;
}

void Renderer::DrawSky(const Texture& texture, const Player& player)
{
	const float fov = (float)M_PI / 3.0f;

	for (size_t x = 0; x < m_Width; x++)
	{
		float angle = (player.GetAngle() - fov / 2.0f) + (float)x / m_Width * fov;
		while (angle > (float)M_PI * 2) angle -= 2 * (float)M_PI;
		while (angle  < (float)-M_PI * 2) angle += 2 * (float)M_PI;
		int xCoord = (int)(angle / (2 * M_PI) * texture.width);
		if (xCoord < 0) xCoord += texture.width;
		xCoord %= texture.width;
		const uint8_t* column = texture.GetColumn(xCoord);
		for (size_t y = 0; y < m_Height / 2; y++)
		{
			int index = (int)((float)y / m_Height * texture.height) * 3;
			int r = column[index + 0];
			int g = column[index + 1];
			int b = column[index + 2];

			Draw(x, y, glm::ivec3(r, g, b));
		}
	}
}

void Renderer::DrawSprite(const Sprite& sprite, const Player& player)
{
	glm::vec2 sprite_dir = sprite.GetPosition() - player.GetPosition();
	float sprite_angle = glm::atan(sprite_dir.y, sprite_dir.x);
	float sprite_distance = glm::length(sprite_dir);
	const float fov = (float)M_PI / 3.0;

	while (sprite_angle - player.GetAngle() > (float)M_PI) sprite_angle -= 2 * (float)M_PI;
	while (sprite_angle - player.GetAngle() < (float)-M_PI) sprite_angle += 2 * (float)M_PI;

	int x_center_of_sprite_on_screen = static_cast<int>((sprite_angle - player.GetAngle() + fov / 2) * m_Width / fov);
	size_t sprite_screen_size = std::min(1000, static_cast<int>(m_Height / sprite_distance));
	size_t sprite_screen_size_scaled = size_t(sprite_screen_size * sprite.GetScale());
	int y_offset = sprite_screen_size / 2 - sprite_screen_size_scaled / 2;
	sprite_screen_size = sprite_screen_size_scaled;
	int top_y = m_Height / 2 - sprite_screen_size / 2;
	if (sprite.IsOnGround())
		top_y += y_offset;

	for (size_t x = 0; x < sprite_screen_size; x++)
	{
		int x_coord_on_screen = x_center_of_sprite_on_screen - sprite_screen_size / 2 + x;
		if (x_coord_on_screen >= (int)m_Width) break;
		if (x_coord_on_screen < 0) continue;
		if (sprite_distance > m_DepthBuffer[x_coord_on_screen]) continue;
		const uint8_t* column = sprite[static_cast<int>((float)x / sprite_screen_size * sprite.GetSize().x)];
		for (size_t y = 0; y < sprite_screen_size; y++)
		{
			int index = static_cast<int>((float)y / sprite_screen_size * sprite.GetSize().y) * 3;
			int r = column[index + 0];
			int g = column[index + 1];
			int b = column[index + 2];

			if (r == 255 && g == 0 && b == 255) continue;
			Draw(x_coord_on_screen,  top_y + y, glm::ivec3(r, g, b));
		}
	}
}

void Renderer::DrawPlayer(const Player& player, int x_offset, int y_offset)
{
	Texture texture = player.GetTexture();

	int WidthPrecentage = m_Width / 10;
	int HeightPrecentage = m_Height / 10;

	for (size_t x = 0; x < texture.width; x++)
	{
		const uint8_t* column = texture.GetColumn(x);
		for(size_t y = 0; y < texture.height; y++)
		{
			int r = column[y * 3 + 0];
			int g = column[y * 3 + 1];
			int b = column[y * 3 + 2];

			if (r == 255 && g == 0 && b == 255) continue; // MAGENTA
			Draw(m_Width / 2 - texture.width / 2 + x + x_offset, m_Height - texture.height + y + y_offset, glm::ivec3(r, g, b));
		}
	}
}

void Renderer::DrawRect(int x, int y, int width, int height, const glm::ivec3& color)
{
	for (int xx = x; xx <= x + width; xx++)
	{
		Draw(xx, y, color);
		Draw(xx, y + height, color);
	}
	for (int yy = y; yy <= y + height; yy++)
	{
		Draw(x, yy, color);
		Draw(x + width, yy, color);
	}
}

void Renderer::FillRect(int x, int y, int width, int height, const glm::ivec3& color)
{
	for (int yy = y; yy <= y + height; yy++)
	{
		for (int xx = x; xx <= x + width; xx++)
		{
			Draw(xx, yy, color);
		}
	}
}

void Renderer::DrawString(int x, int y, std::string str, int fontSize)
{
	const SpriteSheet* fontSheet = ResourceManager::GetSpriteSheet("Font");
	const size_t size = str.size();
	
	for (int i = 0; i < size; i++)
	{
		if(!isalpha(str[i])) continue;
		size_t letterIndex = std::toupper(str[i]) - 'A';
		Texture letterTexture = (*fontSheet)[letterIndex];
		
		for (int xx = 0; xx < fontSize; xx++)
		{
			const uint8_t* column = letterTexture.GetColumn(static_cast<int>((float)xx / fontSize * letterTexture.width));
			for (int yy = 0; yy < fontSize; yy++)
			{
				size_t index = int((float)yy / fontSize * letterTexture.height) * 3;
				int r = column[index + 0];
				int g = column[index + 1];
				int b = column[index + 2];

				if (r == 255 && g == 0 && b == 255) continue; // MAGENTA
				Draw(x + xx + i * fontSize, yy + y, glm::ivec3(r, g, b));
			}
		}
	}
}

void Renderer::DrawHUD(const Player& player)
{
	const int healthBarWidth = 150;
	const int healthBarHeight = 15;
	const glm::ivec3 barColor(130, 20, 10);
	const glm::ivec3 healthColor(240, 15, 0);
	const int mappedHealth = (float)player.GetHealth() / player.GetMaxHealth() * healthBarWidth;
	int xBar = 5;
	int yBar = m_Height - healthBarHeight - 5;

	DrawRect(xBar, yBar, healthBarWidth, healthBarHeight, glm::ivec3(0));

	FillRect(xBar + 1, yBar + 1, healthBarWidth - 2, healthBarHeight - 2, barColor);
	FillRect(xBar + 1, yBar + 1, mappedHealth - 2, healthBarHeight - 2, healthColor);
}

void Renderer::DrawEnemy(const Enemy& enemy, const Player& player)
{
	glm::vec2 dir = enemy.GetPosition() - player.GetPosition();
	float sprite_angle = glm::atan(dir.y, dir.x);
	float sprite_distance = glm::length(dir);
	const float fov = (float)M_PI / 3.0;

	while (sprite_angle - player.GetAngle() > (float)M_PI) sprite_angle -= 2 * (float)M_PI;
	while (sprite_angle - player.GetAngle() < (float)-M_PI) sprite_angle += 2 * (float)M_PI;

	int x_center_of_sprite_on_screen = static_cast<int>((sprite_angle - player.GetAngle() + fov / 2) * m_Width / fov);
	size_t sprite_screen_size = std::min(1000, static_cast<int>(m_Height / sprite_distance));
	size_t sprite_screen_size_scaled = size_t(sprite_screen_size * enemy.GetScaleFactor());
	int y_offset = sprite_screen_size / 2 - sprite_screen_size_scaled / 2;
	sprite_screen_size = sprite_screen_size_scaled;

	int top_y = m_Height / 2 - sprite_screen_size / 2;

	for (size_t x = 0; x < sprite_screen_size; x++)
	{
		// TODO: enemy's sprite is shifted to the right for half of sprite size (???)
		int x_coord_on_screen = x_center_of_sprite_on_screen - sprite_screen_size / 2 + x;
		if (x_coord_on_screen >= (int)m_Width) break;
		if (x_coord_on_screen < 0) continue;
		if (sprite_distance > m_DepthBuffer[x_coord_on_screen]) continue;
		if(enemy.GetState() != EnemyState::Death)
			m_DepthBuffer[x_coord_on_screen] = sprite_distance;
		Texture texture = enemy.GetCurrentTexture();
		const uint8_t* column = texture.GetColumn(static_cast<int>((float)x / sprite_screen_size * texture.width));
		for (size_t y = 0; y < sprite_screen_size; y++)
		{
			int index = static_cast<int>((float)y / sprite_screen_size * texture.height) * 3;
			int r = column[index + 0];
			int g = column[index + 1];
			int b = column[index + 2];

			if (r == 255 && g == 0 && b == 255) continue;
			Draw(x_coord_on_screen, top_y + y + y_offset, glm::ivec3(r, g, b));
		}
	}
}

void Renderer::Flush()
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_Buffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}