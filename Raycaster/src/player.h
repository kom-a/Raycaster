#pragma once

#include <glm/glm.hpp>

#include "spritesheet.h"
#include "camera.h"
#include "map.h"

class Player
{
public:
	Player(float x, float y, float angle, const char* filename);
	~Player();

	void Update(const double& deltaTime, const Map& map);

	inline const glm::vec2& GetPosition() const { return m_Position; }
	inline const float& GetAngle() const { return m_Angle; }
	inline int GetYOffset() const { return m_YOffset; }
	inline const Texture* GetTexture() const { return m_Texture; }

private:
	glm::vec2 m_Position;
	float m_Angle;
	float m_Speed;

	double m_LastMouseX, m_LastMouseY;
	int m_YOffset;

	const Texture* m_Texture;
};
