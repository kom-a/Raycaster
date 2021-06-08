#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "map.h"

Map::Map(const char* filename)
	: m_Width(0), m_Height(0)
{
	std::ifstream infile(filename);
	if (!infile.is_open())
	{
		std::cout << "Failed to open map file\n";
		return;
	}
	
	std::string line;
	std::getline(infile, line);
	m_Data.append(line);
	m_Width = line.length();
	m_Height = 1;

	while (std::getline(infile, line))
	{
		if (line.length() != m_Width)
		{
			std::cout << "Incorrect map file format\n";
			return;
		}
		m_Data.append(line);
		m_Height++;
	}
	infile.close();
}

const char& Map::operator[](size_t index) const
{
	return m_Data[index];
}
