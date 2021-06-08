#include "player.h"

#include "input/input.h"

static constexpr float PI = 3.14159265359f;

Player::Player(float x, float y, float angle)
	: m_Position(x, y), m_Angle(angle), m_Speed(2.5f), m_LastMouseX(0), m_LastMouseY(0), m_YOffset(0)
{
}

Player::~Player()
{
}

void Player::Update(const double& deltaTime)
{
	double speed = m_Speed * deltaTime;
	glm::vec2 moveDirection(0);

	if (Keyboard::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		speed *= 4;
	}
	if (Keyboard::IsKeyPressed(GLFW_KEY_W))
	{
		moveDirection += glm::vec2(glm::cos(m_Angle), glm::sin(m_Angle));
	}
	if (Keyboard::IsKeyPressed(GLFW_KEY_S))
	{
		moveDirection -= glm::vec2(glm::cos(m_Angle), glm::sin(m_Angle));
	}
	if (Keyboard::IsKeyPressed(GLFW_KEY_A))
	{
		float alpha = m_Angle - PI / 2;
		moveDirection += glm::vec2(glm::cos(alpha), glm::sin(alpha));
	}
	if (Keyboard::IsKeyPressed(GLFW_KEY_D))
	{
		float alpha = m_Angle + PI / 2;
		moveDirection += glm::vec2(glm::cos(alpha), glm::sin(alpha));
	}

	m_Angle += (float)(Mouse::GetX() - m_LastMouseX) * 0.0025f;
	m_LastMouseX = Mouse::GetX();
	m_YOffset += m_LastMouseY - Mouse::GetY();
	m_LastMouseY = Mouse::GetY();

	if (moveDirection != glm::vec2(0))
		moveDirection = glm::normalize(moveDirection);

	m_Position += moveDirection * (float)speed;
}

