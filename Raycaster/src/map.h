#pragma once

#include <inttypes.h>
#include <string>

class Map
{
public:
	Map(const char* filename);

	inline uint32_t GetWidth() const { return m_Width; }
	inline uint32_t GetHeight() const { return m_Height; }

	char operator[](size_t index) const;

private:
	uint32_t m_Width, m_Height;
	std::string m_Data;
};