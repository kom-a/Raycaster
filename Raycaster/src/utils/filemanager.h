#pragma once

#include <string>

class FileManager
{
public:
	static std::string ReadFile(const char* filename);
};