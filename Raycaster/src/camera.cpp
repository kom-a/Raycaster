#include "camera.h"

Camera::Camera(float fov)
	: m_FOV(fov)
{

}

Camera::~Camera()
{

}

glm::vec2 Camera::CastRay(const glm::vec2& origin, const glm::vec2& direction, const Map& map, char* tex) const 
{
	glm::vec2 rayUnitStepSize(glm::sqrt(1 + (direction.y / direction.x) * (direction.y / direction.x)), glm::sqrt(1 + (direction.x / direction.y) * (direction.x / direction.y)));

	glm::ivec2 mapCheck(origin.x, origin.y);
	glm::vec2 rayLength1D;

	glm::ivec2 step;

	if (direction.x < 0)
	{
		step.x = -1;
		rayLength1D.x = (origin.x - float(mapCheck.x)) * rayUnitStepSize.x;
	}
	else
	{
		step.x = 1;
		rayLength1D.x = (float(mapCheck.x + 1) - origin.x) * rayUnitStepSize.x;
	}

	if (direction.y < 0)
	{
		step.y = -1;
		rayLength1D.y = (origin.y - float(mapCheck.y)) * rayUnitStepSize.y;
	}
	else
	{
		step.y = 1;
		rayLength1D.y = (float(mapCheck.y + 1) - origin.y) * rayUnitStepSize.y;
	}

	float maxDistance = 60.0f;
	float distance = 0.0f;
	while (true)
	{
		if (rayLength1D.x < rayLength1D.y)
		{
			mapCheck.x += step.x;
			distance = rayLength1D.x;
			rayLength1D.x += rayUnitStepSize.x;
		}
		else
		{
			mapCheck.y += step.y;
			distance = rayLength1D.y;
			rayLength1D.y += rayUnitStepSize.y;
		}

		if (map[mapCheck.y * map.GetWidth() + mapCheck.x] != ' ')
		{
			*tex = map[mapCheck.y * map.GetWidth() + mapCheck.x] - '0';
			return origin + direction * distance;
		}
	}

	return glm::vec2(0, 0);
}