#include <iostream>

#include "shader.h"

#include "utils/filemanager.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	m_Shader = glCreateProgram();
	GLuint vertex = CompileShader(vertexPath, GL_VERTEX_SHADER);
	GLuint fragment = CompileShader(fragmentPath, GL_FRAGMENT_SHADER);
	glAttachShader(m_Shader, vertex);
	glAttachShader(m_Shader, fragment);
	glLinkProgram(m_Shader);
	CheckError(m_Shader, GL_LINK_STATUS);
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
	glDeleteProgram(m_Shader);
}

void Shader::Bind() const
{
	glUseProgram(m_Shader);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

GLuint Shader::CompileShader(const char* path, GLenum type)
{
	GLuint shader = glCreateShader(type);
		
	std::string source = FileManager::ReadFile(path);
	const char* src = source.c_str();
	glShaderSource(shader, 1, &src, 0);
	glCompileShader(shader);
	CheckError(shader, GL_COMPILE_STATUS);

	return shader;
}

void Shader::CheckError(GLuint object, GLenum status)
{
	switch (status)
	{
		case GL_COMPILE_STATUS:
		{
			int success;
			glGetShaderiv(object, status, &success);
			if (!success)
			{
				int length;
				glGetShaderiv(object, GL_INFO_LOG_LENGTH, &length);
				char* infoLog = (char*)alloca(length);
				glGetShaderInfoLog(object, length, &length, infoLog);
				std::cout << infoLog << std::endl;
			}
		} break;
		case GL_LINK_STATUS:
		{
			int success;
			glGetProgramiv(object, status, &success);
			if (!success)
			{
				int length;
				glGetProgramiv(object, GL_INFO_LOG_LENGTH, &length);
				char* infoLog = (char*)alloca(length);
				glGetProgramInfoLog(object, length, &length, infoLog);
				std::cout << infoLog << std::endl;
			}
		} break;
		default:
		{
			std::cout << "No such status." << std::endl;
		}
	}
}

void Shader::SetUniform1i(const char* name, int value)
{
	if (m_LocationsCache.find(name) == m_LocationsCache.end())
	{
		int location = glGetUniformLocation(m_Shader, name);
		m_LocationsCache.insert(std::make_pair(name, location));
		if (location == -1)
			std::cout << "Could not find uniform \"" << name << "\"" << std::endl;
	}
	glUniform1i(m_LocationsCache.at(name), value);
}

void Shader::SetUniform1f(const char* name, float value)
{
	if (m_LocationsCache.find(name) == m_LocationsCache.end())
	{
		int location = glGetUniformLocation(m_Shader, name);
		m_LocationsCache.insert(std::make_pair(name, location));
		if (location == -1)
			std::cout << "Could not find uniform \"" << name << "\"" << std::endl;
	}
	glUniform1f(m_LocationsCache.at(name), value);
}

void Shader::SetUniform2f(const char* name, float v1, float v2)
{
	if (m_LocationsCache.find(name) == m_LocationsCache.end())
	{
		int location = glGetUniformLocation(m_Shader, name);
		m_LocationsCache.insert(std::make_pair(name, location));
		if (location == -1)
			std::cout << "Could not find uniform \"" << name << "\"" << std::endl;
	}
	glUniform2f(m_LocationsCache.at(name), v1, v2);
}

void Shader::SetUniform3f(const char* name, float v1, float v2, float v3)
{
	if (m_LocationsCache.find(name) == m_LocationsCache.end())
	{
		int location = glGetUniformLocation(m_Shader, name);
		m_LocationsCache.insert(std::make_pair(name, location));
		if (location == -1)
			std::cout << "Could not find uniform \"" << name << "\"" << std::endl;
	}
	glUniform3f(m_LocationsCache.at(name), v1, v2, v3);
}

void Shader::SetUniform4f(const char* name, float v1, float v2, float v3, float v4)
{
	if (m_LocationsCache.find(name) == m_LocationsCache.end())
	{
		int location = glGetUniformLocation(m_Shader, name);
		m_LocationsCache.insert(std::make_pair(name, location));
		if (location == -1)
			std::cout << "Could not find uniform \"" << name << "\"" << std::endl;
	}
	glUniform4f(m_LocationsCache.at(name), v1, v2, v3, v4);
}

void Shader::SetUniform2f(const char* name, const glm::vec2& vec)
{
	SetUniform2f(name, vec.x, vec.y);
}

void Shader::SetUniform3f(const char* name, const glm::vec3& vec)
{
	SetUniform3f(name, vec.x, vec.y, vec.z);
}

void Shader::SetUniform4f(const char* name, const glm::vec4& vec)
{
	SetUniform4f(name, vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetMatrix4(const char* name, const glm::mat4& matrix)
{
	if (m_LocationsCache.find(name) == m_LocationsCache.end())
	{
		int location = glGetUniformLocation(m_Shader, name);
		m_LocationsCache.insert(std::make_pair(name, location));
		if (location == -1)
			std::cout << "Could not find uniform \"" << name << "\"" << std::endl;
	}
	glUniformMatrix4fv(m_LocationsCache.at(name), 1, GL_FALSE, &matrix[0][0]);
}
