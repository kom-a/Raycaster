#include "player.h"

#include "input/input.h"
#include "utils/filemanager.h"

static constexpr float PI = 3.14159265359f;

Player::Player(float x, float y, float angle, const char* filename)
	: m_Position(x, y), m_Angle(angle), m_Speed(2.5f), m_LastMouseX(0), m_LastMouseY(0), m_YOffset(0)
{
	m_Texture = FileManager::LoadTexture(filename);
}

Player::~Player()
{
	delete m_Texture;
}

void Player::Update(const double& deltaTime, const Map& map)
{
	double speed = m_Speed * deltaTime;
	glm::vec2 velocity(0);

	if (Keyboard::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		speed *= 1.5;
	}
	if (Keyboard::IsKeyPressed(GLFW_KEY_W))
	{
		velocity += glm::vec2(glm::cos(m_Angle), glm::sin(m_Angle));
	}
	if (Keyboard::IsKeyPressed(GLFW_KEY_S))
	{
		velocity -= glm::vec2(glm::cos(m_Angle), glm::sin(m_Angle));
	}
	if (Keyboard::IsKeyPressed(GLFW_KEY_A))
	{
		float alpha = m_Angle - PI / 2;
		velocity += glm::vec2(glm::cos(alpha), glm::sin(alpha));
	}
	if (Keyboard::IsKeyPressed(GLFW_KEY_D))
	{
		float alpha = m_Angle + PI / 2;
		velocity += glm::vec2(glm::cos(alpha), glm::sin(alpha));
	}

	m_Angle += (float)(Mouse::GetX() - m_LastMouseX) * 0.0025f;
	m_LastMouseX = Mouse::GetX();
	m_YOffset += (int)(m_LastMouseY - Mouse::GetY());
	m_LastMouseY = Mouse::GetY();

	if (velocity != glm::vec2(0))
		velocity = glm::normalize(velocity) * float(speed);
	
	glm::vec2 potentionalPosition = m_Position + velocity;

	glm::ivec2 currentCell = glm::floor(m_Position);
	glm::ivec2 targetCell = potentionalPosition;
	glm::ivec2 areaTL = glm::max(glm::min(currentCell, targetCell) - glm::ivec2(1, 1), glm::ivec2(0, 0));
	glm::ivec2 areaBR = glm::min(glm::max(currentCell, targetCell) + glm::ivec2(1, 1), glm::ivec2(map.GetWidth(), map.GetHeight()));

	float radius = 0.1f;

	glm::ivec2 cell;
	for (cell.y = areaTL.y; cell.y <= areaBR.y; cell.y++)
	{
		for (cell.x = areaTL.x; cell.x <= areaBR.x; cell.x++)
		{
			if (map[cell.y * map.GetWidth() + cell.x] != ' ')
			{
				glm::vec2 nearestPoint;
				nearestPoint.x = glm::max(float(cell.x), glm::min(potentionalPosition.x, float(cell.x + 1)));
				nearestPoint.y = glm::max(float(cell.y), glm::min(potentionalPosition.y, float(cell.y + 1)));
				
				glm::vec2 rayToNearest = nearestPoint - potentionalPosition;
				float overlap = radius - glm::length(rayToNearest);

				if (std::isnan(overlap))	overlap = 0;

				if (overlap > 0)
				{
					potentionalPosition = potentionalPosition - glm::normalize(rayToNearest) * overlap;
				}
			}
		}
	}

	m_Position = potentionalPosition;
}