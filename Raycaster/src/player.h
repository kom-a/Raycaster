#pragma once

#include <glm/glm.hpp>

#include "spritesheet.h"
#include "camera.h"
#include "map.h"
#include "enemies/enemy.h"

class Player
{
public:
	Player(float x, float y, float angle, const char* filename, int textureWidth, int textureHeight);
	~Player();

	void Update(const double& deltaTime, const Map& map, const std::vector<Enemy*>& enemies);

	inline const glm::vec2& GetPosition() const { return m_Position; }
	inline const float& GetAngle() const { return m_Angle; }
	inline int GetYOffset() const { return m_YOffset; }
	inline Texture GetTexture() const { return m_SpriteSheet->operator[](m_CurrentAnim); }
	inline const Camera* GetCamera() const { return m_Camera; }

private:
	glm::vec2 m_Position;
	float m_Angle;
	float m_Speed;

	Camera* m_Camera;

	double m_LastMouseX, m_LastMouseY;
	int m_YOffset;

	bool m_ReadyToShoot;
	int m_CurrentAnim;
	double m_AnimationTime;
	const SpriteSheet* m_SpriteSheet;
	
	int m_Health;
	int m_Damage;
};
