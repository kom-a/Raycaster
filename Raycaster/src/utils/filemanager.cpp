#include "FileManager.h"

#include <iostream>
#include <assert.h>

std::string FileManager::ReadFile(const char* filepath)
{
	FILE* file;
	if (fopen_s(&file, filepath, "r"))
	{
		std::cout << "Failed to open \"" << filepath << "\"" << std::endl;
		assert(0);
		return "";
	}
	fseek(file, 0, SEEK_END);
	long length = ftell(file);
	char* data = new char[length + 1];
	memset(data, 0, length + 1);
	fseek(file, 0, SEEK_SET);
	fread(data, sizeof(char), length, file);
	fclose(file);

	std::string source(data);
	delete[] data;

	return source;
}

uint8_t* FileManager::ConvertToColumnMajor(const uint8_t* data, int width, int height, int comp)
{
	uint8_t* columnMajor = new uint8_t[width * height * comp];

	int i = 0;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			for (int c = 0; c < comp; c++)
			{
				columnMajor[i * comp + c] = data[(y * width + x) * comp + c];
			}
			i++;
		}
	}
	return columnMajor;
}
