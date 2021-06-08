#pragma once

class Camera
{
public:
	Camera(float fov);
	~Camera();

private:
	float m_FOV; // Field of view
};