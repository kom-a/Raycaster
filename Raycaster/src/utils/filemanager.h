#pragma once

#include <string>
#include <cstdint>

class FileManager
{
public:
	static std::string ReadFile(const char* filename);
	static uint8_t* ConvertToColumnMajor(const uint8_t* data, int width, int height, int comp);
};