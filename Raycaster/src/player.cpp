#include "player.h"

#include <iostream>

#include "input/input.h"
#include "utils/filemanager.h"
// #include "window.h"

static constexpr float PI = 3.14159265359f;

Player::Player(float x, float y, float angle, const char* filename, int textureWidth, int textureHeight)
	: m_Position(x, y),
	m_Angle(angle),
	m_Speed(2.5f),
	m_LastMouseX(0),
	m_LastMouseY(0),
	m_YOffset(0),
	m_CurrentAnim(0),
	m_AnimationTime(0),
	m_ReadyToShoot(true),
	m_Health(100),
	m_Damage(10)
{
	m_Camera = new Camera(PI / 3);
	m_SpriteSheet = new SpriteSheet(filename, textureWidth, textureHeight, false);
}

Player::~Player()
{
	delete m_Camera;
	delete m_SpriteSheet;
}

void Player::Update(const double& deltaTime, const Map& map, const std::vector<Enemy*>& enemies)
{
	double speed = m_Speed * deltaTime;
	glm::vec2 velocity(0);

	m_AnimationTime += deltaTime;

	if (m_ReadyToShoot && Mouse::IsButtonPressed(GLFW_MOUSE_BUTTON_LEFT))
	{
		m_ReadyToShoot = false;

		for (Enemy* e : enemies)
		{
			if(e->GetState() == EnemyState::Death) continue;
			glm::vec2 enemyDir = e->GetPosition() - this->GetPosition();
			float sprite_distance = glm::length(enemyDir);

			char hittedTexture;
			glm::vec2 intersectionPoint = m_Camera->CastRay(m_Position, glm::vec2(glm::cos(m_Angle), glm::sin(m_Angle)), map, &hittedTexture);
			float rayToWallLength = glm::length(intersectionPoint - m_Position);
			if (rayToWallLength < sprite_distance) continue;

			float sprite_angle = glm::atan(enemyDir.y, enemyDir.x);
			const float fov = PI / 3.0;

			while (sprite_angle > 2 * PI) sprite_angle -= 2 * PI;
			while (sprite_angle < 0) sprite_angle += 2 * PI;

			float player_angle = m_Angle;
			while (player_angle > 2 * PI) player_angle -= 2 * PI;
			while (player_angle < 0) player_angle += 2 * PI;
			
			// TODO: Rewrite this
			int windowWidth = 640;
			int windowHeight = windowWidth * 9 / 16;

			int x_center_of_sprite_on_screen = static_cast<int>((sprite_angle - player_angle + fov / 2) * windowWidth / fov);
			size_t sprite_screen_size = std::min(1000, static_cast<int>(windowHeight / sprite_distance));
			size_t sprite_screen_size_scaled = size_t(sprite_screen_size * e->GetScaleFactor());
			
			float angleMin = (this->GetAngle() - fov / 2.0f) + (float)(x_center_of_sprite_on_screen - sprite_screen_size_scaled / 2) / windowWidth * fov;
			float angleMax = (this->GetAngle() - fov / 2.0f) + (float)(x_center_of_sprite_on_screen + sprite_screen_size_scaled / 2) / windowWidth * fov;

			while (angleMin - this->GetAngle() > PI) angleMin -= 2 * PI;
			while (angleMin - this->GetAngle() < -PI) angleMin += 2 * PI;

			while (angleMax - this->GetAngle() > PI) angleMax -= 2 * PI;
			while (angleMax - this->GetAngle() < -PI) angleMax += 2 * PI;

			if (m_Angle > angleMin && m_Angle < angleMax)
			{
				int randomDamage = rand() % 15;
				e->TakeHit(m_Damage + randomDamage);
				break;
			}
		}
	}

	if (!m_ReadyToShoot)
	{
		if (m_AnimationTime >= 0.05)
		{
			m_CurrentAnim++;
			if (m_CurrentAnim >= 6)
			{
				m_ReadyToShoot = true;
				m_CurrentAnim = 0;
			}
			m_AnimationTime = 0;
		}
	}
	
	if (Keyboard::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
	{
		speed *= 2;
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

	// Collision detection
	glm::ivec2 currentCell = glm::floor(m_Position);
	glm::ivec2 targetCell = potentionalPosition;
	glm::ivec2 areaTL = glm::max(glm::min(currentCell, targetCell) - glm::ivec2(1, 1), glm::ivec2(0, 0));
	glm::ivec2 areaBR = glm::min(glm::max(currentCell, targetCell) + glm::ivec2(1, 1), glm::ivec2(map.GetWidth(), map.GetHeight()));

	float radius = 0.3f;

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