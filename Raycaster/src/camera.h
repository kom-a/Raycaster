#pragma once

#include <glm/glm.hpp>
#include "map.h"

class Camera
{
public:
	Camera(float fov);
	~Camera();

	// direction must be normalized
	glm::vec2 CastRay(const glm::vec2& origin, const glm::vec2& direction, const Map& map, char* tex);

	inline const float& GetFOV() const { return m_FOV; }
private:
	float m_FOV; // Field of view
};