#pragma once

#include <cstdint>
#include "glm/glm.hpp"

class Sprite
{
public:
	Sprite(const char* filename, int x, int y, float scale, bool onGrount);
	~Sprite();

	const uint8_t* operator[](size_t index) const;

	inline const glm::ivec2& GetSize() const { return m_Size; }
	inline const glm::vec2& GetPosition() const { return m_Position; }
	inline const uint8_t* GetPixels() const { return m_Pixels; }

	inline void SetPosition(const glm::vec2 new_position) { m_Position = new_position; }
	inline float GetScale() const { return m_Scale; }
	inline bool IsOnGround() const { return m_OnGround; }
private:
	glm::vec2 m_Position;
	glm::ivec2 m_Size;
	uint8_t* m_Pixels;
	float m_Scale;
	bool m_OnGround;
};